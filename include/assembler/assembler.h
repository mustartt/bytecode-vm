//
// Created by Admininstrator on 2023-12-24.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_

#include <optional>
#include <ostream>
#include <vector>
#include <unordered_map>

#include "src_loc.h"

namespace vm {

class token_type {
  public:
    enum type : uint8_t {
        eof = 0,
        whitespace = 1,
        comment = 2,
        label = 3,
        identifier = 4,
        numeric = 5,
        constpool = 6,
        newline = 7,
        string = 8,
        section = 9,
        include = 10
    };
  public:
    constexpr token_type() : t(eof) {}
    explicit token_type(type t) : t(t) {}
    [[nodiscard]] type get() const { return t; }
  private:
    type t;
};

using token = std::tuple<token_type, std::string, src_loc>;

class instruction {
  public:
    instruction(std::string op, std::vector<token> operands)
        : op(std::move(op)), operands(std::move(operands)) {}
  public:
    [[nodiscard]] const std::string &get_op() const noexcept { return op; }
    [[nodiscard]] const std::vector<token> &get_operands() const noexcept { return operands; }
    [[nodiscard]] const token &get_operands(int idx) const noexcept { return operands[idx]; }
    [[nodiscard]] size_t size() const noexcept { return operands.size(); }

  private:
    std::string op;
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
  private:
    std::vector<instruction> instructions;
    std::unordered_map<label, offset> labels;
    std::unordered_map<offset, std::vector<std::string>> comments;
};

class module {
  public:
    explicit module(std::string module_name) : module_name(std::move(module_name)) {}
    std::vector<std::string> &paths() noexcept { return include_paths; }
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

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
