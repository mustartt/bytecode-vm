//
// Created by Admininstrator on 2023-12-24.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_

#include <optional>
#include <vector>
#include <unordered_map>
#include <map>

#include "token.h"

namespace vm {

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

class bytecode_format {
  private:
    class symbol_entry {
      public:
        symbol_entry()
            : resolved(false), const_offset(0), data_offset(0) {};
        symbol_entry(bool resolved, size_t constOffset, size_t dataOffset)
            : resolved(resolved), const_offset(constOffset), data_offset(dataOffset) {};
        bool operator==(const symbol_entry &other) const {
            return other.resolved == resolved &&
                other.const_offset == const_offset &&
                other.data_offset == data_offset;
        }
      public:
        bool resolved;
        std::size_t const_offset;
        std::size_t data_offset;
    };
  public:
    void serialize(std::ostream &os);
    void deserialize(std::istream &is);

  public:
    std::string metadata;
    std::vector<std::string> dependency;
    std::map<std::string, symbol_entry> symbol_table;
    std::vector<uint8_t> data;
    std::vector<uint64_t> constpool;
    std::map<std::size_t, std::string> comments;
};

class assembler {

};

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
