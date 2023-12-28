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
    void emit_const_i32(uint16_t index);
    void emit_add_i32();
    void emit_sub_i32();
    void emit_mul_i32();
    void emit_div_i32();
    void emit_rem_i32();
    void emit_pop_i32();
    void emit_call(uint16_t arg_size);
    void emit_ret();
    void emit_load_addr(uint16_t index);
    void emit_load_rel_i32(uint16_t offset);

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
