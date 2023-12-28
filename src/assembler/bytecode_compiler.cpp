//
// Created by henry on 12/27/2023.
//

#include "assembler/bytecode_compiler.h"

#include <queue>

#include "boost/lexical_cast.hpp"

#define REGISTER_HANDLER(keyword, handler) \
    instr_desc[#keyword] = [this](const instruction &instr) { \
        handler(instr); \
    } \

#define EXPECT_ARGS(instr, n_args) \
    if (instr.size() != n_args) { \
        std::ostringstream oss; \
        oss << "bytecode_compiler: invalid number of arguments "; \
        oss << instr.size(); \
        oss << " expected " << n_args << " at "; \
        oss << instr.get_src_loc(); \
        throw std::runtime_error(oss.str()); \
    } \

#define EXPECT_ARG_TYPE(instr, idx, type) \
    if (std::get<0>(instr.get_operands(idx)).get() != type) { \
        std::ostringstream oss; \
        oss << "bytecode_compiler: expected token type <"; \
        oss << #type; \
        oss << "> at " << std::get<2>(instr.get_operands(idx)); \
        throw std::runtime_error(oss.str()); \
    } \

#define PARSE_ARG_AS(instr, result, idx, type) \
    try { \
        result = boost::lexical_cast<type>( \
            std::get<1>(instr.get_operands(idx))); \
    } catch (boost::bad_lexical_cast &e) { \
        std::ostringstream oss; \
        oss << "bytecode_compiler: unable to parse "; \
        oss << std::get<1>(instr.get_operands(idx)); \
        oss << " as " << #type; \
        oss << " at "; \
        oss << std::get<2>(instr.get_operands(idx)); \
        throw std::runtime_error(oss.str()); \
    } \

namespace vm {

bytecode_compiler::bytecode_compiler(std::istream &is) : token_stream(is), p(token_stream) {
    REGISTER_HANDLER(const, compile_const);
    REGISTER_HANDLER(reserve, compile_reserve);

    REGISTER_HANDLER(halt, compile_halt);
    REGISTER_HANDLER(const_i32, compile_const_i32);
    REGISTER_HANDLER(add_i32, compile_add_i32);
    REGISTER_HANDLER(sub_i32, compile_sub_i32);
    REGISTER_HANDLER(mul_i32, compile_mul_i32);
    REGISTER_HANDLER(div_i32, compile_div_i32);
    REGISTER_HANDLER(rem_i32, compile_rem_i32);
    REGISTER_HANDLER(pop_i32, compile_pop_i32);
    REGISTER_HANDLER(load_rel_i32, compile_load_rel_i32);
    REGISTER_HANDLER(call, compile_call);
    REGISTER_HANDLER(ret, compile_ret);
    REGISTER_HANDLER(load_addr, compile_load_addr);
}

void bytecode_compiler::compile_module(std::ostream &os) {
    module mod = p.parse_module();
    a.register_module_name(mod.get_module_name());

    bytecode_format &bytecode = a.get_bytecode();
    for (const auto &include: mod.get_includes()) {
        bytecode.dependency.push_back(include);
    }

    compile_code(mod.get_sections().at("text"));
    compile_data(mod.get_sections().at("data"));
    compile_data(mod.get_sections().at("readonly"));

    bytecode.serialize(os);
}

void bytecode_compiler::compile_code(const section &sect) {
    using label_pair = std::pair<size_t, std::string>;
    using comment_pair = std::pair<size_t, std::string>;
    std::priority_queue<label_pair, std::vector<label_pair>, std::greater<>> labels;
    std::priority_queue<comment_pair, std::vector<comment_pair>, std::greater<>> comments;

    for (const auto &[label, offset]: sect.get_labels()) {
        labels.emplace(offset, label);
    }
    for (const auto &[offset, strings]: sect.get_comments()) {
        std::string joined;
        for (const auto &s: strings) {
            joined += s;
            joined += ' ';
        }
        labels.emplace(offset, std::move(joined));
    }

    const auto &instrs = sect.get_instructions();
    for (size_t offset = 0; offset < instrs.size(); ++offset) {
        while (!labels.empty() && labels.top().first <= offset) {
            a.register_label(labels.top().second);
            labels.pop();
        }
        while (!comments.empty() && comments.top().first <= offset) {
            a.register_label(comments.top().second);
            comments.pop();
        }

        compile_instr(instrs[offset]);
    }
}

void bytecode_compiler::compile_instr(const instruction &instr) {
    if (!instr_desc.contains(instr.get_op())) {
        throw std::runtime_error("bytecode_compiler: unknown instruction op " + instr.get_op());
    }
    instr_desc[instr.get_op()](instr); // call the bound handler for the instruction
}

void bytecode_compiler::compile_halt(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_halt();
}

void bytecode_compiler::compile_const_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 1);
    uint16_t index;
    EXPECT_ARG_TYPE(instr, 0, token_type::offset);
    PARSE_ARG_AS(instr, index, 0, uint16_t);
    a.emit_const_i32(index);
}

void bytecode_compiler::compile_add_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_add_i32();
}

void bytecode_compiler::compile_sub_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_sub_i32();
}

void bytecode_compiler::compile_mul_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_mul_i32();
}

void bytecode_compiler::compile_div_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_div_i32();
}

void bytecode_compiler::compile_rem_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_rem_i32();
}

void bytecode_compiler::compile_pop_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_pop_i32();
}

void bytecode_compiler::compile_load_rel_i32(const instruction &instr) {
    EXPECT_ARGS(instr, 1);

    int16_t offset;
    EXPECT_ARG_TYPE(instr, 0, token_type::numeric);
    PARSE_ARG_AS(instr, offset, 0, int16_t);

    throw std::runtime_error("not implemented"); // todo: implement this
}

void bytecode_compiler::compile_call(const instruction &instr) {
    EXPECT_ARGS(instr, 1);
    uint16_t arg_size;
    EXPECT_ARG_TYPE(instr, 0, token_type::numeric);
    PARSE_ARG_AS(instr, arg_size, 0, uint16_t);
    a.emit_call(arg_size);
}

void bytecode_compiler::compile_ret(const instruction &instr) {
    EXPECT_ARGS(instr, 0);
    a.emit_ret();
}

void bytecode_compiler::compile_load_addr(const instruction &instr) {
    EXPECT_ARGS(instr, 1);
    uint16_t index;
    EXPECT_ARG_TYPE(instr, 0, token_type::offset);
    PARSE_ARG_AS(instr, index, 0, uint16_t);
    a.emit_load_addr(index);
}

void bytecode_compiler::compile_data(const section &sect) {

}

void bytecode_compiler::compile_const(const section &sect) {

}

void bytecode_compiler::compile_const(const instruction &instr) {

}

void bytecode_compiler::compile_reserve(const instruction &instr) {

}

}
