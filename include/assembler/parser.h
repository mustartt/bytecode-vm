//
// Created by henry on 12/24/2023.
//

#ifndef BYTECODE_VM_SRC_ASSEMBLER_PARSER_H_
#define BYTECODE_VM_SRC_ASSEMBLER_PARSER_H_

#include "tokenizer.h"

namespace vm {

class parser {
  public:
    explicit parser(tokenizer &tokenStream) : token_stream(tokenStream) {
        auto first = token_stream.next();
        if (first) {
            lookahead = first.value();
        }
    }

  public:
    module parse_module();
    section parse_section();
    void parse_instr(section &s);
    void parse_label(section &s);

  private:
    void next() {
        auto tok = token_stream.next();
        while (tok && std::get<0>(tok.value()).get() == token_type::whitespace) {
            tok = token_stream.next();
        }
        if (tok) {
            lookahead = tok.value();
        } else {
            std::get<0>(lookahead) = token_type(token_type::eof);
        }
    }

    void consume_newline() {
        std::vector<std::string> comments;
        consume_newline_comment(comments);
    }

    void consume_newline_comment(std::vector<std::string> &comments) {
        if (!expect(token_type::newline) && !expect(token_type::comment)) {
            throw std::runtime_error("expected <line-return> " + get_src_loc().str());
        }
        while (expect(token_type::newline) || expect(token_type::comment)) {
            if (expect(token_type::comment)) {
                comments.push_back(get_lexeme());
            }
            next();
        }
    }

    [[nodiscard]] bool expect(token_type::type type) const {
        return type == std::get<0>(lookahead).get();
    }
    [[nodiscard]] const std::string &get_lexeme() const {
        return std::get<1>(lookahead);
    }
    [[nodiscard]] const src_loc &get_src_loc() const {
        return std::get<2>(lookahead);
    }
  private:
    token lookahead;
    tokenizer &token_stream;
};

}

#endif //BYTECODE_VM_SRC_ASSEMBLER_PARSER_H_
