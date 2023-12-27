//
// Created by henry on 12/27/2023.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_TOKEN_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_TOKEN_H_

#include <ostream>

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
        offset = 6,
        newline = 7,
        string = 8,
        section = 9,
        include = 10
    };
  public:
    constexpr token_type() : t(eof) {}
    explicit token_type(type t) : t(t) {}
    [[nodiscard]] type get() const { return t; }
    [[nodiscard]] std::string str() const;
  private:
    type t;
};

using token = std::tuple<token_type, std::string, src_loc>;

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_TOKEN_H_
