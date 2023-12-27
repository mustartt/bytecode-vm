//
// Created by henry on 12/24/2023.
//

#include "assembler/tokenizer.h"

#include "boost/assert.hpp"

namespace vm {

tokenizer::tokenizer(std::istream &stream) : stream(stream) {
    patterns.emplace_back(token_type::newline, "^\\n()");
    patterns.emplace_back(token_type::whitespace, "^(\\s+)");
    patterns.emplace_back(token_type::comment, "^;\\s(.+)");
    patterns.emplace_back(token_type::section, "^(section)");
    patterns.emplace_back(token_type::include, "^(include)");
    patterns.emplace_back(token_type::label, R"(^([_a-zA-Z][a-zA-Z0-9_\(\)\[\]\<\>]*):)");
    patterns.emplace_back(token_type::identifier, R"(^([_a-zA-Z][a-zA-Z0-9_\(\)\[\]\<\>]+))");
    patterns.emplace_back(token_type::constpool, "^\\$(\\d+)");
    patterns.emplace_back(token_type::numeric, R"(^([-+]?\d+(\.\d+)?([eE][-+]?\d+)?))");
    patterns.emplace_back(token_type::string, R"(^\"(.*)\")");
}

std::optional<token> tokenizer::next() {
    if (line.empty()) {
        auto good = consume_next_line();
        if (!good) return std::nullopt;
        line += '\n';
    }
    return consume_token();
}

bool tokenizer::consume_next_line() {
    ++curr_line;
    curr_col = 1;
    auto &res = std::getline(stream, line);
    return res.good();
}

token tokenizer::consume_token() {
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

}
