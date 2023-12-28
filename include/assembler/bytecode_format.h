//
// Created by henry on 12/28/2023.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_BYTECODE_FORMAT_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_BYTECODE_FORMAT_H_

#include <vector>
#include <unordered_map>
#include <map>
#include <iostream>

namespace vm {

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

class bytecode_format {
  public:
    bytecode_format() = default;
    bytecode_format(const bytecode_format &other) noexcept = delete;
    bytecode_format(bytecode_format &&other) noexcept = default;
    bytecode_format &operator=(const bytecode_format &other) noexcept = delete;
    bytecode_format &operator=(bytecode_format &&other) noexcept = default;

  public:
    void serialize(std::ostream &os);
    void deserialize(std::istream &is);

    const std::string &module_name() const { return metadata; }

  public:
    std::string metadata;
    std::vector<std::string> dependency;
    std::map<std::string, symbol_entry> symbol_table;
    std::vector<uint8_t> data;
    std::vector<uint64_t> constpool;
    std::map<std::size_t, std::string> data_comments;
    std::map<std::size_t, std::string> const_comments;
};

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_BYTECODE_FORMAT_H_
