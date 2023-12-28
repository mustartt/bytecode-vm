#include <iostream>
#include <fstream>

#include "vm/virtual_machine.h"
#include "assembler/bytecode_compiler.h"

using namespace vm;

int main(int argc, char *argv[]) {
    using namespace vm;

    std::ifstream file("../test/example/example2.asm");
    std::stringstream ss;

    bytecode_compiler compiler(file);
    compiler.compile_module(ss);

    bytecode_format bytecode;
    bytecode.deserialize(ss);

    vm_context ctx(4);
    virtual_machine vm(ctx);
    vm.load_module(bytecode);

    int ec = vm.start_execution();

    BOOST_LOG_TRIVIAL(trace) << "vm finished with exit code " << ec;
    return ec;
}
