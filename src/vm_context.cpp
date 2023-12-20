//
// Created by henry on 12/18/2023.
//

#include "vm_context.h"

namespace vm {

using namespace boost;

void vm::vm_context::worker_thread() {
    BOOST_LOG_TRIVIAL(trace) << "vm: worker thread (id: "
                             << std::this_thread::get_id()
                             << ") started";

    fibers::use_scheduling_algorithm<fibers::algo::work_stealing>(thread_count);

    std::unique_lock lk(mtx_fiber_count);
    cnd_fiber_count.wait(lk, [this]() { return fiber_count == 0; });
    BOOST_ASSERT(fiber_count == 0);
}

}
