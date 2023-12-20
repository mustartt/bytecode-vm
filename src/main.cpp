
#include "virtual_machine.h"

int main(int argc, char *argv[]) {
    using namespace vm;

    virtual_machine::memory pool{
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00,
    };
    int val = 123;
    std::memcpy(&pool[0] + 4, &val, sizeof(val));
    val = 456;
    std::memcpy(&pool[0] + 8, &val, sizeof(val));

    virtual_machine::memory data{
        Bytecode::const_i32, 0x00, 0x00,
        Bytecode::const_i32, 0x01, 0x00,
        Bytecode::const_i32, 0x02, 0x00,
        Bytecode::add_i32,
        Bytecode::pop_i32,
        Bytecode::halt,
    };

    vm_context ctx(4);
    virtual_machine vm(ctx, std::move(pool), std::move(data));
    int ec = vm.start_execution();

    return ec;
}
