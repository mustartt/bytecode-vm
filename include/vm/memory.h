//
// Created by henry on 12/28/2023.
//

#ifndef BYTECODE_VM_INCLUDE_VM_MEMORY_H_
#define BYTECODE_VM_INCLUDE_VM_MEMORY_H_

#include <cstdint>
#include <cstring>
#include <algorithm>

namespace vm {

class memory final {
  public:
    explicit memory(std::size_t bytes) : bytes(bytes) {
        base = new uint8_t[bytes];
    }
    memory(uint8_t *base, std::size_t bytes) : base(base), bytes(bytes) {}
    memory(const memory &other) noexcept = delete;
    memory &operator=(const memory &other) noexcept = delete;
    memory(memory &&other) noexcept {
        base = other.base;
        bytes = other.bytes;
        other.base = nullptr;
        other.bytes = 0;
    };
    memory &operator=(memory &&other) noexcept {
        if (this == &other) return *this;
        base = other.base;
        bytes = other.bytes;
        other.base = nullptr;
        other.bytes = 0;
        return *this;
    }
    ~memory() {
        delete[] base;
    }
  public:
    uint8_t *data() const noexcept { return base; }
    void resize(std::size_t new_size) {
        auto new_ptr = new uint8_t[new_size];
        std::memcpy(new_ptr, base, std::min(bytes, new_size));
        delete base;
        base = new_ptr;
        bytes = new_size;
    }
    std::ptrdiff_t get_offset(uint8_t *other) const noexcept {
        return other - base;
    }
    uint8_t *from_offset(std::ptrdiff_t offset) const noexcept {
        return base + offset;
    }
  private:
    uint8_t *base;
    std::size_t bytes;
};

using cpool = std::vector<uint64_t>;

}

#endif //BYTECODE_VM_INCLUDE_VM_MEMORY_H_
