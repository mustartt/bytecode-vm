
#include "virtual_machine.h"

int main(int argc, char *argv[]) {
    using namespace vm;

    virtual_machine::cpool pool{
        0,
        1,
        2,
        12, // offset into the main memory to add()
    };

    virtual_machine::memory data{
        // main
        Bytecode::const_i32, 0x01, 0x00,
        Bytecode::const_i32, 0x02, 0x00,

        Bytecode::load_addr, 0x03, 0x00,
        Bytecode::call, 0x8,

        Bytecode::halt,

        // add(int a, int b) int
        Bytecode::add_i32,
        Bytecode::ret,
    };

    vm_context ctx(4);
    virtual_machine vm(ctx, std::move(pool), std::move(data));
    int ec = vm.start_execution();

    BOOST_LOG_TRIVIAL(trace) << "vm finished with exit code " << ec;
    return ec;
}
