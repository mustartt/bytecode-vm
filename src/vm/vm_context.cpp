//
// Created by henry on 12/18/2023.
//

#include "vm/vm_context.h"

namespace vm {

using namespace boost;

vm_context::vm_context(std::size_t thread_count)
    : thread_count(thread_count) {
    for (std::size_t i = 1; i < thread_count; ++i) {
        pool.emplace_back([this] { worker_thread(); });
    }
}

void vm_context::start_parent(std::function<void()> func) {
    using namespace boost;
    BOOST_LOG_TRIVIAL(trace) << "vm: main thread (id: "
                             << std::this_thread::get_id()
                             << ") started";
    fibers::use_scheduling_algorithm<fibers::algo::work_stealing>(thread_count);

    boost::fibers::fiber f([this, &func] {
        setup_fiber(true, func);
    });
    f.join();

    {
        std::unique_lock lk(mtx_fiber_count);
        cnd_fiber_count.wait(lk, [this]() { return fiber_count == 0; });
    }

    BOOST_ASSERT(0 == fiber_count);
    for (auto &t: pool) {
        t.join();
    }
}

void vm::vm_context::worker_thread() {
    BOOST_LOG_TRIVIAL(trace) << "vm: worker thread (id: "
                             << std::this_thread::get_id()
                             << ") started";

    fibers::use_scheduling_algorithm<fibers::algo::work_stealing>(thread_count);

    std::unique_lock lk(mtx_fiber_count);
    cnd_fiber_count.wait(lk, [this]() { return fiber_count == 0; });
    BOOST_ASSERT(fiber_count == 0);
}

void vm_context::setup_fiber(bool is_main, std::function<void()> &func) {
    if (!is_main) {
        std::unique_lock lk(mtx_fiber_count);
        ++fiber_count;
    }

    func();

    std::unique_lock lk(mtx_fiber_count);
    if (--fiber_count == 0) {
        lk.unlock();
        cnd_fiber_count.notify_all();
    }
}

}
