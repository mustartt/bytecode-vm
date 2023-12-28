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

class vm_context {
  public:
    explicit vm_context(std::size_t thread_count = std::thread::hardware_concurrency());

  public:
    void start_parent(std::function<void()> func);

  private:
    void worker_thread();
    void setup_fiber(bool is_main, std::function<void()> &func);

  private:
    std::vector<std::thread> pool;
    std::size_t thread_count;

    std::size_t fiber_count = 1;
    std::mutex mtx_fiber_count;
    boost::fibers::condition_variable_any cnd_fiber_count;
};

}

#endif //BYTECODE_VM_INCLUDE_VM_CONTEXT_H_
