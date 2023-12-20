//
// Created by henry on 12/18/2023.
//

#ifndef BYTECODE_VM_INCLUDE_VM_CONTEXT_H_
#define BYTECODE_VM_INCLUDE_VM_CONTEXT_H_

#include <vector>
#include <thread>

#include "boost/fiber/condition_variable.hpp"
#include "boost/log/trivial.hpp"
#include "boost/fiber/algo/work_stealing.hpp"

namespace vm {

class exec_context {

};

class vm_context {
  public:
    explicit vm_context(std::size_t thread_count = std::thread::hardware_concurrency())
        : thread_count(thread_count) {
        for (std::size_t i = 1; i < thread_count; ++i) {
            pool.emplace_back([this] { worker_thread(); });
        }
    }

  public:
    void start_parent(std::function<void()> func) {
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

  private:
    void worker_thread();

    void setup_fiber(bool is_main, std::function<void()> &func) {
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

  private:
    std::vector<std::thread> pool;
    std::size_t thread_count;

    std::size_t fiber_count = 1;
    std::mutex mtx_fiber_count;
    boost::fibers::condition_variable_any cnd_fiber_count;
};

}

#endif //BYTECODE_VM_INCLUDE_VM_CONTEXT_H_
