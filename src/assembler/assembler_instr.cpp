//
// Created by henry on 12/28/2023.
//

#include "common/common.h"
#include "assembler/assembler.h"

namespace vm {

// vm instr
void assembler::emit_halt() {
    write_unaligned_data8(Bytecode::halt);
}

void assembler::emit_nop() {
    write_unaligned_data8(Bytecode::nop);
}

// primitive: i8
void assembler::emit_const_i8(uint16_t index) {
    write_unaligned_data8(Bytecode::const_i8);
    write_unaligned_data16(index);
}

void assembler::emit_convert_i8_i32() {
    write_unaligned_data8(Bytecode::convert_i8_i32);
}

// primitive: i16
void assembler::emit_const_i16(uint16_t index) {
    write_unaligned_data8(Bytecode::const_i16);
    write_unaligned_data16(index);
}

void assembler::emit_convert_i16_i32() {
    write_unaligned_data8(Bytecode::convert_i16_i32);
}

// primitive: i32
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

void assembler::emit_load_i32() {
    write_unaligned_data8(Bytecode::load_i32);
}

void assembler::emit_load_offset_i32(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_offset_i32);
    write_unaligned_data16(offset);
}

void assembler::emit_load_array_i32() {
    write_unaligned_data8(Bytecode::load_array_i32);
}

void assembler::emit_load_local_i32(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_local_i32);
    write_unaligned_data16(offset);
}

void assembler::emit_store_i32() {
    write_unaligned_data8(Bytecode::store_i32);
}

void assembler::emit_store_offset_i32(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_offset_i32);
    write_unaligned_data16(offset);
}

void assembler::emit_store_array_i32() {
    write_unaligned_data8(Bytecode::store_array_i32);
}

void assembler::emit_store_local_i32(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_local_i32);
    write_unaligned_data16(offset);
}

void assembler::emit_convert_i32_i8() {
    write_unaligned_data8(Bytecode::convert_i32_i8);
}

void assembler::emit_convert_i32_i16() {
    write_unaligned_data8(Bytecode::convert_i32_i16);
}

void assembler::emit_convert_i32_i64() {
    write_unaligned_data8(Bytecode::convert_i32_i64);
}

void assembler::emit_convert_i32_f32() {
    write_unaligned_data8(Bytecode::convert_i32_f32);
}

void assembler::emit_convert_i32_f64() {
    write_unaligned_data8(Bytecode::convert_i32_f64);
}

void assembler::emit_cmp_i32() {
    write_unaligned_data8(Bytecode::cmp_i32);
}

// primitive: i64
void assembler::emit_const_i64(uint16_t index) {
    write_unaligned_data8(Bytecode::const_i64);
    write_unaligned_data16(index);
}

void assembler::emit_add_i64() {
    write_unaligned_data8(Bytecode::add_i64);
}

void assembler::emit_sub_i64() {
    write_unaligned_data8(Bytecode::sub_i64);
}

void assembler::emit_mul_i64() {
    write_unaligned_data8(Bytecode::mul_i64);
}

void assembler::emit_div_i64() {
    write_unaligned_data8(Bytecode::div_i64);
}

void assembler::emit_rem_i64() {
    write_unaligned_data8(Bytecode::rem_i64);
}

void assembler::emit_pop_i64() {
    write_unaligned_data8(Bytecode::pop_i64);
}

void assembler::emit_load_i64() {
    write_unaligned_data8(Bytecode::load_i64);
}

void assembler::emit_load_offset_i64(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_offset_i64);
    write_unaligned_data16(offset);
}

void assembler::emit_load_array_i64() {
    write_unaligned_data8(Bytecode::load_array_i64);
}

void assembler::emit_load_local_i64(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_local_i64);
    write_unaligned_data16(offset);
}

void assembler::emit_store_i64() {
    write_unaligned_data8(Bytecode::store_i64);
}

void assembler::emit_store_offset_i64(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_offset_i64);
    write_unaligned_data16(offset);
}

void assembler::emit_store_array_i64() {
    write_unaligned_data8(Bytecode::store_array_i64);
}

void assembler::emit_store_local_i64(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_local_i64);
    write_unaligned_data16(offset);
}

void assembler::emit_convert_i64_i32() {
    write_unaligned_data8(Bytecode::convert_i64_i32);
}

void assembler::emit_convert_i64_f32() {
    write_unaligned_data8(Bytecode::convert_i64_f32);
}

void assembler::emit_convert_i64_f64() {
    write_unaligned_data8(Bytecode::convert_i64_f64);
}

void assembler::emit_cmp_i64() {
    write_unaligned_data8(Bytecode::cmp_i64);
}

// primitive: f32
void assembler::emit_const_f32(uint16_t index) {
    write_unaligned_data8(Bytecode::const_f32);
    write_unaligned_data16(index);
}

void assembler::emit_add_f32() {
    write_unaligned_data8(Bytecode::add_f32);
}

void assembler::emit_sub_f32() {
    write_unaligned_data8(Bytecode::sub_f32);
}

void assembler::emit_mul_f32() {
    write_unaligned_data8(Bytecode::mul_f32);
}

void assembler::emit_div_f32() {
    write_unaligned_data8(Bytecode::div_f32);
}

void assembler::emit_rem_f32() {
    write_unaligned_data8(Bytecode::rem_f32);
}

void assembler::emit_pop_f32() {
    write_unaligned_data8(Bytecode::pop_f32);
}

void assembler::emit_load_f32() {
    write_unaligned_data8(Bytecode::load_f32);
}

void assembler::emit_load_offset_f32(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_offset_f32);
    write_unaligned_data16(offset);
}

void assembler::emit_load_array_f32() {
    write_unaligned_data8(Bytecode::load_array_f32);
}

void assembler::emit_load_local_f32(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_local_f32);
    write_unaligned_data16(offset);
}

void assembler::emit_store_f32() {
    write_unaligned_data8(Bytecode::store_f32);
}

void assembler::emit_store_offset_f32(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_offset_f32);
    write_unaligned_data16(offset);
}

void assembler::emit_store_array_f32() {
    write_unaligned_data8(Bytecode::store_array_f32);
}

void assembler::emit_store_local_f32(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_local_f32);
    write_unaligned_data16(offset);
}

void assembler::emit_convert_f32_i32() {
    write_unaligned_data8(Bytecode::convert_f32_i32);
}

void assembler::emit_convert_f32_i64() {
    write_unaligned_data8(Bytecode::convert_f32_i64);
}

void assembler::emit_convert_f32_f64() {
    write_unaligned_data8(Bytecode::convert_f32_f64);
}

void assembler::emit_cmp_f32() {
    write_unaligned_data8(Bytecode::cmp_f32);
}

// primitive: f64
void assembler::emit_const_f64(uint16_t index) {
    write_unaligned_data8(Bytecode::const_f64);
    write_unaligned_data16(index);
}

void assembler::emit_add_f64() {
    write_unaligned_data8(Bytecode::add_f64);
}

void assembler::emit_sub_f64() {
    write_unaligned_data8(Bytecode::sub_f64);
}

void assembler::emit_mul_f64() {
    write_unaligned_data8(Bytecode::mul_f64);
}

void assembler::emit_div_f64() {
    write_unaligned_data8(Bytecode::div_f64);
}

void assembler::emit_rem_f64() {
    write_unaligned_data8(Bytecode::rem_f64);
}

void assembler::emit_pop_f64() {
    write_unaligned_data8(Bytecode::pop_f64);
}

void assembler::emit_load_f64() {
    write_unaligned_data8(Bytecode::load_f64);
}

void assembler::emit_load_offset_f64(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_offset_f64);
    write_unaligned_data16(offset);
}

void assembler::emit_load_array_f64() {
    write_unaligned_data8(Bytecode::load_array_f64);
}

void assembler::emit_load_local_f64(uint16_t offset) {
    write_unaligned_data8(Bytecode::load_local_f64);
    write_unaligned_data16(offset);
}

void assembler::emit_store_f64() {
    write_unaligned_data8(Bytecode::store_f64);
}

void assembler::emit_store_offset_f64(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_offset_f64);
    write_unaligned_data16(offset);
}

void assembler::emit_store_array_f64() {
    write_unaligned_data8(Bytecode::store_array_f64);
}

void assembler::emit_store_local_f64(uint16_t offset) {
    write_unaligned_data8(Bytecode::store_local_f64);
    write_unaligned_data16(offset);
}

void assembler::emit_convert_f64_i32() {
    write_unaligned_data8(Bytecode::convert_f64_i32);
}

void assembler::emit_convert_f64_i64() {
    write_unaligned_data8(Bytecode::convert_f64_i64);
}

void assembler::emit_convert_f64_f32() {
    write_unaligned_data8(Bytecode::convert_f64_f32);
}

void assembler::emit_cmp_f64() {
    write_unaligned_data8(Bytecode::cmp_f64);
}

// basic control flow
void assembler::emit_branch(uint16_t index) {
    write_unaligned_data8(Bytecode::branch);
    write_unaligned_data16(index);
}

void assembler::emit_if_eq0_i32() {
    write_unaligned_data8(Bytecode::if_eq0_i32);
}

void assembler::emit_if_ne0_i32() {
    write_unaligned_data8(Bytecode::if_ne0_i32);
}

void assembler::emit_if_lt0_i32() {
    write_unaligned_data8(Bytecode::if_lt0_i32);
}

void assembler::emit_if_ge0_i32() {
    write_unaligned_data8(Bytecode::if_ge0_i32);
}

void assembler::emit_if_gte0_i32() {
    write_unaligned_data8(Bytecode::if_gte0_i32);
}

void assembler::emit_if_lte0_i32() {
    write_unaligned_data8(Bytecode::if_lte0_i32);
}

void assembler::emit_if_cmp_eq_i32() {
    write_unaligned_data8(Bytecode::if_cmp_eq_i32);
}

void assembler::emit_if_cmp_ne_i32() {
    write_unaligned_data8(Bytecode::if_cmp_ne_i32);
}

void assembler::emit_if_cmp_lt_i32() {
    write_unaligned_data8(Bytecode::if_cmp_lt_i32);
}

void assembler::emit_if_cmp_gt_i32() {
    write_unaligned_data8(Bytecode::if_cmp_gt_i32);
}

void assembler::emit_if_cmp_lte_i32() {
    write_unaligned_data8(Bytecode::if_cmp_lte_i32);
}

void assembler::emit_if_cmp_gte_i32() {
    write_unaligned_data8(Bytecode::if_cmp_gte_i32);
}

}

//void assembler::emit_call(uint16_t arg_size) {
//    write_unaligned_data8(Bytecode::call);
//    write_unaligned_data16(arg_size);
//}
//
//void assembler::emit_ret() {
//    write_unaligned_data8(Bytecode::ret);
//}
//
//void assembler::emit_load_addr(uint16_t index) {
//    write_unaligned_data8(Bytecode::load_addr);
//    write_unaligned_data16(index);
//}
