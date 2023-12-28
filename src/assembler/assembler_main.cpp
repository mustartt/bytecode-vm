#include <fstream>
#include "assembler/bytecode_compiler.h"

using namespace vm;

int main() {
    std::ifstream file("../test/example/example1.asm");
    std::ofstream output("../test/example/example1.output");

    bytecode_compiler compiler(file);
    compiler.compile_module(output);

    return 0;
}
