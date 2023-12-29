//
// Created by Admininstrator on 2023-12-24.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_

#include "token.h"
#include "bytecode_format.h"

namespace vm {

class instruction {
  public:
    instruction(std::string op, src_loc loc, std::vector<token> operands)
        : op(std::move(op)), loc(std::move(loc)), operands(std::move(operands)) {}
  public:
    [[nodiscard]] const std::string &get_op() const noexcept { return op; }
    [[nodiscard]] const src_loc &get_src_loc() const noexcept { return loc; }
    [[nodiscard]] const std::vector<token> &get_operands() const noexcept { return operands; }
    [[nodiscard]] const token &get_operands(int idx) const noexcept { return operands[idx]; }
    [[nodiscard]] size_t size() const noexcept { return operands.size(); }

  private:
    std::string op;
    src_loc loc;
    std::vector<token> operands;
};

class section {
  public:
    using label = std::string;
    using offset = std::size_t;
  public:
    void insert_comments(std::string &&comment) {
        comments[current_offset()].push_back(std::move(comment));
    }
    [[nodiscard]] std::size_t current_offset() const { return instructions.size(); }
    void insert_instr(instruction &&instr) {
        instructions.push_back(std::move(instr));
    }
    void insert_label(const label &label) {
        if (labels.contains(label)) {
            throw std::runtime_error("duplicate section name");
        }
        labels[label] = current_offset();
    }
    const auto &get_labels() const { return labels; }
    const auto &get_instructions() const { return instructions; }
    const auto &get_comments() const { return comments; }

  private:
    std::vector<instruction> instructions;
    std::unordered_map<label, offset> labels;
    std::unordered_map<offset, std::vector<std::string>> comments;
};

class module {
  public:
    explicit module(std::string module_name) : module_name(std::move(module_name)) {}
    std::vector<std::string> &paths() noexcept { return include_paths; }
    [[nodiscard]] const std::string &get_module_name() const noexcept { return module_name; }
    [[nodiscard]] const std::vector<std::string> &get_includes() const noexcept { return include_paths; }
    [[nodiscard]] const std::unordered_map<std::string, section> &
    get_sections() const noexcept { return sections; }

    void add_section(const std::string &name, section sect) {
        if (sections.contains(name)) {
            throw std::runtime_error("duplicate section name");
        }
        sections[name] = std::move(sect);
    }
  private:
    std::string module_name;
    std::vector<std::string> include_paths;
    std::unordered_map<std::string, section> sections;
};

constexpr uint64_t Assembler_Alignment = 8;

class assembler {
  public:
    explicit assembler() {
        bytecode.data.reserve(1);
    };
  public:
    bytecode_format &get_bytecode() { return bytecode; }

    void register_module_name(const std::string &name) {
        bytecode.metadata = "module_name: " + name;
    }

    // instructions
    void emit_halt();
    void emit_nop();

    // primitive: i8
    void emit_const_i8(uint16_t index);
    void emit_convert_i8_i32();

    // primitive: i16
    void emit_const_i16(uint16_t index);
    void emit_convert_i16_i32();

    // primitive: i32
    void emit_const_i32(uint16_t index);
    void emit_add_i32();
    void emit_sub_i32();
    void emit_mul_i32();
    void emit_div_i32();
    void emit_rem_i32();
    void emit_pop_i32();
    void emit_load_i32();
    void emit_load_offset_i32(uint16_t offset);
    void emit_load_array_i32();
    void emit_load_local_i32(uint16_t offset);
    void emit_store_i32();
    void emit_store_offset_i32(uint16_t offset);
    void emit_store_array_i32();
    void emit_store_local_i32(uint16_t offset);
    void emit_convert_i32_i8();
    void emit_convert_i32_i16();
    void emit_convert_i32_i64();
    void emit_convert_i32_f32();
    void emit_convert_i32_f64();
    void emit_cmp_i32();

    // primitive: i64
    void emit_const_i64(uint16_t index);
    void emit_add_i64();
    void emit_sub_i64();
    void emit_mul_i64();
    void emit_div_i64();
    void emit_rem_i64();
    void emit_pop_i64();
    void emit_load_i64();
    void emit_load_offset_i64(uint16_t offset);
    void emit_load_array_i64();
    void emit_load_local_i64(uint16_t offset);
    void emit_store_i64();
    void emit_store_offset_i64(uint16_t offset);
    void emit_store_array_i64();
    void emit_store_local_i64(uint16_t offset);
    void emit_convert_i64_i32();
    void emit_convert_i64_f32();
    void emit_convert_i64_f64();
    void emit_cmp_i64();

    // primitive: f32
    void emit_const_f32(uint16_t index);
    void emit_add_f32();
    void emit_sub_f32();
    void emit_mul_f32();
    void emit_div_f32();
    void emit_rem_f32();
    void emit_pop_f32();
    void emit_load_f32();
    void emit_load_offset_f32(uint16_t offset);
    void emit_load_array_f32();
    void emit_load_local_f32(uint16_t offset);
    void emit_store_f32();
    void emit_store_offset_f32(uint16_t offset);
    void emit_store_array_f32();
    void emit_store_local_f32(uint16_t offset);
    void emit_convert_f32_i32();
    void emit_convert_f32_i64();
    void emit_convert_f32_f64();
    void emit_cmp_f32();

    // primitive: f64
    void emit_const_f64(uint16_t index);
    void emit_add_f64();
    void emit_sub_f64();
    void emit_mul_f64();
    void emit_div_f64();
    void emit_rem_f64();
    void emit_pop_f64();
    void emit_load_f64();
    void emit_load_offset_f64(uint16_t offset);
    void emit_load_array_f64();
    void emit_load_local_f64(uint16_t offset);
    void emit_store_f64();
    void emit_store_offset_f64(uint16_t offset);
    void emit_store_array_f64();
    void emit_store_local_f64(uint16_t offset);
    void emit_convert_f64_i32();
    void emit_convert_f64_i64();
    void emit_convert_f64_f32();
    void emit_cmp_f64();

    // basic control flow
    void emit_branch(uint16_t index);

    void emit_if_eq0_i32();
    void emit_if_ne0_i32();
    void emit_if_lt0_i32();
    void emit_if_ge0_i32();
    void emit_if_gte0_i32();
    void emit_if_lte0_i32();

    void emit_if_cmp_eq_i32();
    void emit_if_cmp_ne_i32();
    void emit_if_cmp_lt_i32();
    void emit_if_cmp_gt_i32();
    void emit_if_cmp_lte_i32();
    void emit_if_cmp_gte_i32();

    // function control flow
    void emit_call(uint16_t arg_size);
    void emit_call_native(uint16_t arg_size);
    void emit_ret();

    // other
    void emit_load_addr(uint16_t index);

    // data
    void write_unaligned_data8(uint8_t value) { write_data(&value, sizeof(value)); }
    void write_unaligned_data16(uint16_t value) { write_data(&value, sizeof(value)); }
    void write_unaligned_data32(uint32_t value) { write_data(&value, sizeof(value)); }
    void write_unaligned_data64(uint64_t value) { write_data(&value, sizeof(value)); }

    void reserve_aligned_data_var(const std::string &identifier, std::size_t bytes);

    // const pool
    void write_const_i32(std::size_t index, int32_t value);
    void write_const_addr(std::size_t index, uint64_t value);
    void write_const_addr(std::size_t index, const std::string &label);
    void write_const_str(std::size_t index, const std::string &value);

    // other
    void register_label(const std::string &label);
    void register_data_comment(const std::string &comment);
    void register_const_comment(std::size_t index, const std::string &comment);

  private:
    void *reserve_data_bytes(std::size_t bytes);
    void reserve_const(std::size_t index);
    void write_data(void *value, std::size_t size);
    [[nodiscard]] std::size_t data_offset() const { return bytecode.data.size(); }
    void align_data();

  private:
    bytecode_format bytecode;
};

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
