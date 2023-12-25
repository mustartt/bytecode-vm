//
// Created by Admininstrator on 2023-12-24.
//

#ifndef BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
#define BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_

#include <istream>
#include <optional>
#include <ostream>
#include <regex>

#include "boost/assert.hpp"

namespace vm {

class src_loc {
  public:
    constexpr src_loc() noexcept: start_line(0), start_col(0),
                                  end_line(0), end_col(0) {}
    src_loc(size_t start_line, size_t start_col,
            size_t end_line, size_t end_col)
        : start_line(start_line), start_col(start_col),
          end_line(end_line), end_col(end_col) {}

    friend std::ostream &operator<<(std::ostream &os, const src_loc &loc) {
        os << "(start: " << loc.start_line << ":" << loc.start_col << " end: " << loc.end_line
           << ":" << loc.end_col << ")";
        return os;
    }
  public:
    size_t start_line, start_col;
    size_t end_line, end_col;
};

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
        line_return = 7,
        string = 8,
    };
  public:
    explicit token_type(type t) : t(t) {}
    [[nodiscard]] type get() const { return t; }
  private:
    type t;
};

class tokenizer {
  public:
    using token = std::tuple<token_type, std::string, src_loc>;

  public:
    explicit tokenizer(std::istream &stream) : stream(stream) {
        patterns.emplace_back(token_type::line_return, "^\\n()");
        patterns.emplace_back(token_type::whitespace, "^(\\s+)");
        patterns.emplace_back(token_type::comment, "^;\\s(.+)");
        patterns.emplace_back(token_type::label, R"(^([_a-zA-Z][a-zA-Z0-9_\(\)\[\]\<\>]*):)");
        patterns.emplace_back(token_type::identifier, R"(^([_a-zA-Z][a-zA-Z0-9_\(\)\[\]\<\>]+))");
        patterns.emplace_back(token_type::constpool, "^\\$(\\d+)");
        patterns.emplace_back(token_type::numeric, R"(^([-+]?\d+(\.\d+)?([eE][-+]?\d+)?))");
        patterns.emplace_back(token_type::string, R"(^\"(.*)\")");
    }

    std::optional<token> next() {
        if (line.empty()) {
            auto good = consume_next_line();
            if (!good) return std::nullopt;
            line += '\n';
        }
        return consume_token();
    }

  private:
    bool consume_next_line() {
        ++curr_line;
        curr_col = 1;
        auto &res = std::getline(stream, line);
        return res.good();
    }

    token consume_token() {
        BOOST_ASSERT_MSG(!line.empty(), "current line is not empty");
        std::smatch match_result;
        for (const auto &[type, pat]: patterns) {
            if (std::regex_search(line, match_result, pat)) {
                BOOST_ASSERT_MSG(match_result.size() >= 2, "insufficient capture group");
                size_t token_len = match_result[0].length();
                src_loc token_loc(curr_line, curr_col, curr_line, curr_col + token_len - 1);
                curr_col += token_len;
                std::string parsed = match_result[1].str();
                line = line.substr(token_len);
                return std::make_tuple<>(type, parsed, token_loc);
            }
        }
        throw std::runtime_error("unable to parse token: [" + line + "]");
    }

  private:
    size_t curr_line = 0;
    size_t curr_col = 1;

    std::vector<std::pair<token_type, std::regex>> patterns;
    std::string line;

    std::istream &stream;
};

}

#endif //BYTECODE_VM_INCLUDE_ASSEMBLER_ASSEMBLER_H_
