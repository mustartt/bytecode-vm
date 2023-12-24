//
// Created by henry on 12/21/2023.
//

#ifndef BYTECODE_VM_INCLUDE_COMMON_SHARED_OBJECT_H_
#define BYTECODE_VM_INCLUDE_COMMON_SHARED_OBJECT_H_

#include <atomic>
#include <functional>
#include <boost/assert.hpp>

namespace vm {

class weak_ref;

namespace {
class control_block {
  public:
    constexpr control_block() : strong(1), weak(1) {}

  public:
    void inc_strong() { strong.fetch_add(1, std::memory_order_relaxed); }
    void inc_weak() { weak.fetch_add(1, std::memory_order_relaxed); }
    int32_t dec_strong() { return strong.fetch_sub(1, std::memory_order_release); }
    int32_t dec_weak() { return weak.fetch_sub(1, std::memory_order_release); }
    int32_t use_count() { return strong.load(std::memory_order_relaxed); }

  public:
    std::atomic<int32_t> strong;
    std::atomic<int32_t> weak;
};
}

class strong_ref {
  public:
    constexpr strong_ref() noexcept: ref(nullptr) {}

    friend strong_ref make_object(std::size_t);
    friend class weak_ref;
  public:
    [[nodiscard]] void *get_buf() const { return get_buf_offset(ref); }
    [[nodiscard]] int32_t use_count() const { return get_ctrlblk().use_count(); }
    explicit operator bool() const noexcept { return ref != nullptr; }

    void acquire(const strong_ref &other) noexcept {
        BOOST_ASSERT_MSG(ref == nullptr, "strong_ref::acquire can only be used on empty strong_ref");
        ref = other.ref;
        if (ref) {
            get_ctrlblk().inc_strong();
        }
    }

    void release(std::function<void(void *)> &&deleter) noexcept {
        if (!ref) return;
        if (get_ctrlblk().dec_strong() == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            deleter(get_buf()); // destruct the managed object
            if (get_ctrlblk().dec_weak() == 1) {
                std::atomic_thread_fence(std::memory_order_acquire);
                operator delete(ref); // free memory after all weak_ref are released
            }
        }
        ref = nullptr;
    }

  private:
    static inline void *get_buf_offset(void *base) noexcept {
        return static_cast<uint8_t *>(base) + sizeof(control_block);
    }
    [[nodiscard]] control_block &get_ctrlblk() const noexcept {
        return *static_cast<control_block *>(ref);
    }

  private:
    void *ref;
};

class weak_ref {
  public:
    constexpr weak_ref() noexcept: ref(nullptr) {}
  public:
    [[nodiscard]] int32_t use_count() const { return get_ctrlblk().use_count(); }
    [[nodiscard]] bool expired() const noexcept { return use_count() == 0; }
    explicit operator bool() const noexcept { return ref != nullptr; }

    void from(const strong_ref &other) noexcept {
        BOOST_ASSERT_MSG(ref == nullptr, "weak_ref::from can only be used on empty weak_ref");
        ref = other.ref;
        get_ctrlblk().inc_weak();
    }

    strong_ref lock() noexcept {
        if (!ref) return {};
        int32_t old = get_ctrlblk().strong.load(std::memory_order_relaxed);
        do {
            if (old == 0) return {}; // no more strong ref
        } while (
            !get_ctrlblk().strong.compare_exchange_weak(
                old, old + 1,
                std::memory_order_acq_rel,
                std::memory_order_relaxed)
            );
        // compare exchange success
        strong_ref ptr;
        ptr.ref = ref;
        return ptr;
    }

    void acquire(const weak_ref &other) noexcept {
        BOOST_ASSERT_MSG(ref == nullptr, "weak_ref::acquire can only be used on empty weak_ref");
        ref = other.ref;
        if (ref) {
            get_ctrlblk().inc_weak();
        }
    }

    void release() noexcept {
        if (!ref) return;
        if (get_ctrlblk().dec_weak() == 1) {
            std::atomic_thread_fence(std::memory_order_acquire);
            operator delete(ref);
        }
        ref = nullptr;
    }

  private:
    [[nodiscard]] control_block &get_ctrlblk() const noexcept {
        return *static_cast<control_block *>(ref);
    }
  private:
    void *ref;
};

strong_ref make_object(std::size_t size) {
    auto base = operator new(sizeof(control_block) + size);
    new(base) control_block(); // placement new construct control_block at the base
    strong_ref ptr;
    ptr.ref = base;
    return ptr;
}

}

#endif //BYTECODE_VM_INCLUDE_COMMON_SHARED_OBJECT_H_
