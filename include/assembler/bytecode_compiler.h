//
// Created by henry on 12/27/2023.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_BYTECODE_COMPILER_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_BYTECODE_COMPILER_H_

#include "tokenizer.h"
#include "parser.h"

namespace vm {

class bytecode_compiler {
  public:
    explicit bytecode_compiler(std::istream &is);

  public:
    void compile_module(std::ostream &os);
    void compile_code(const section &sect);

    void compile_instr(const instruction &instr);

    void compile_const(const instruction &instr);
    void compile_reserve(const instruction &instr);

    void compile_halt(const instruction &instr);
    void compile_const_i32(const instruction &instr);
    void compile_add_i32(const instruction &instr);
    void compile_sub_i32(const instruction &instr);
    void compile_mul_i32(const instruction &instr);
    void compile_div_i32(const instruction &instr);
    void compile_rem_i32(const instruction &instr);
    void compile_pop_i32(const instruction &instr);
    void compile_load_rel_i32(const instruction &instr);
    void compile_call(const instruction &instr);
    void compile_ret(const instruction &instr);
    void compile_load_addr(const instruction &instr);

  private:
    tokenizer token_stream;
    parser par;
    assembler builder;
    std::unordered_map<
        std::string,
        std::function<void(const instruction &instr)>
    > instr_desc;
    std::unordered_map<std::string, size_t> type_size;
};

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_BYTECODE_COMPILER_H_
