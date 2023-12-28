//
// Created by henry on 12/27/2023.
//

#include <cstring>

#include "common.h"
#include "assembler/assembler.h"

namespace vm {

void *assembler::reserve_data_bytes(std::size_t bytes) {
    auto &ref = bytecode.data;
    for (std::size_t i = 0; i < bytes; ++i) {
        ref.push_back(0);
    }
    return &ref[ref.size() - bytes];
}

void assembler::write_data(void *value, std::size_t size) {
    void *base = reserve_data_bytes(size);
    std::memcpy(base, value, size);
}

void assembler::align_data() {
    auto padding = Assembler_Alignment - (bytecode.data.size() % Assembler_Alignment);
    reserve_data_bytes(padding % Assembler_Alignment);
}

void assembler::emit_halt() {
    write_unaligned_data8(Bytecode::halt);
}

void assembler::emit_const_i32(uint16_t index) {
    write_unaligned_data8(Bytecode::const_i32);
    write_unaligned_data16(index);
}

void assembler::emit_add_i32() {
    write_unaligned_data8(Bytecode::add_i32);
}

void assembler::emit_sub_i32() {
    write_unaligned_data8(Bytecode::sub_i32);
}

void assembler::emit_mul_i32() {
    write_unaligned_data8(Bytecode::mul_i32);
}

void assembler::emit_div_i32() {
    write_unaligned_data8(Bytecode::div_i32);
}

void assembler::emit_rem_i32() {
    write_unaligned_data8(Bytecode::rem_i32);
}

void assembler::emit_pop_i32() {
    write_unaligned_data8(Bytecode::pop_i32);
}

void assembler::emit_call(uint16_t arg_size) {
    write_unaligned_data8(Bytecode::call);
    write_unaligned_data16(arg_size);
}

void assembler::emit_ret() {
    write_unaligned_data8(Bytecode::ret);
}

void assembler::load_addr(uint16_t index) {
    write_unaligned_data8(Bytecode::load_addr);
    write_unaligned_data16(index);
}

void assembler::load_rel_i32(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_rel_i32);
    write_unaligned_data16(offset);
}

void assembler::register_label(const std::string &symbol) {
    if (bytecode.symbol_table.contains(symbol)) {
        auto &entry = bytecode.symbol_table[symbol];
        if (entry.resolved) {
            throw std::runtime_error("assembler: duplicate symbol is already resolved " + symbol);
        }
        entry.resolved = true;
        bytecode.constpool[entry.const_offset] = data_offset();
        entry.data_offset = data_offset();
    } else {
        bytecode.symbol_table[symbol] = symbol_entry(true, 0, data_offset());
    }
}

void assembler::write_const_i32(std::size_t index, int32_t value) {
    reserve_const(index);
    bytecode.constpool[index] = value;
}

void assembler::write_const_addr(std::size_t index, uint64_t value) {
    reserve_const(index);
    bytecode.constpool[index] = value;
}

void assembler::write_const_addr(std::size_t index, const std::string &label) {
    reserve_const(index);
    if (bytecode.symbol_table.contains(label)) {
        auto &entry = bytecode.symbol_table.at(label);
        bytecode.constpool[index] = entry.resolved ? entry.data_offset : 0;
    } else {
        bytecode.symbol_table[label] = symbol_entry(false, index, 0);
    }
}

void assembler::write_const_str(std::size_t index, const std::string &value) {
    reserve_const(index);
    align_data();
    auto len = value.size() + 1;
    auto base = reserve_data_bytes(len);
    std::memcpy(base, value.c_str(), len);
}

void assembler::reserve_const(std::size_t index) {
    while (bytecode.constpool.size() <= index) {
        bytecode.constpool.push_back(0);
    }
}

void assembler::reserve_aligned_data_var(const std::string &identifier, std::size_t bytes) {
    align_data();
    register_label(identifier);
    reserve_data_bytes(bytes);
}

void assembler::register_data_comment(const std::string &comment) {
    bytecode.data_comments[data_offset()] = comment;
}

void assembler::register_const_comment(std::size_t index, const std::string &comment) {
    reserve_const(index);
    bytecode.const_comments[index] = comment;
}

}
