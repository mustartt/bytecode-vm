//
// Created by henry on 12/24/2023.
//

#ifndef BYTECODE_VM_SRC_ASSEMBLER_TOKENIZER_H_
#define BYTECODE_VM_SRC_ASSEMBLER_TOKENIZER_H_

#include "assembler.h"

#include <regex>

namespace vm {

class tokenizer {
  public:
    explicit tokenizer(std::istream &stream);
  public:
    std::optional<token> next();

  private:
    bool consume_next_line();
    token consume_token();

  private:
    size_t curr_line = 0;
    size_t curr_col = 1;
    std::string line;

    std::vector<std::pair<token_type, std::regex>> patterns;
    std::istream &stream;
};

}

#endif //BYTECODE_VM_SRC_ASSEMBLER_TOKENIZER_H_
