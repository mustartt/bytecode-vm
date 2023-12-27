//
// Created by henry on 12/25/2023.
//

#include "assembler/parser.h"

namespace vm {

module parser::parse_module() {
    if (!expect(token_type::identifier)) {
        throw std::runtime_error("module: expected <identifier> " + get_src_loc().str());
    }
    if (get_lexeme() != "module") {
        throw std::runtime_error("module: expected keyword <module> " + get_src_loc().str());
    }

    next();
    if (!expect(token_type::identifier)) {
        throw std::runtime_error("module: expected <identifier> " + get_src_loc().str());
    }
    std::string module_name = get_lexeme();
    module parsed_module(std::move(module_name));

    next(); // newline
    consume_newline();

    while (expect(token_type::include)) {
        next();
        if (!expect(token_type::string)) {
            throw std::runtime_error("include: expected <string> " + get_src_loc().str());
        }
        parsed_module.paths().push_back(get_lexeme());
        next();
        consume_newline();
    }

    while (expect(token_type::section)) {
        next();
        std::string section_name = get_lexeme();
        next();
        consume_newline();

        auto parsed_section = parse_section();
        parsed_module.add_section(section_name, std::move(parsed_section));
    }

    if (!expect(token_type::eof)) {
        throw std::runtime_error("module: did not reach eof " + get_src_loc().str());
    }

    return parsed_module;
}

section parser::parse_section() {
    section sect;
    while (expect(token_type::label) || expect(token_type::identifier)) {
        if (expect(token_type::label)) {
            parse_label(sect);
        } else {
            parse_instr(sect);
        }
    }
    return sect;
}

void parser::parse_instr(section &s) {
    if (!expect(token_type::identifier)) {
        throw std::runtime_error("instr: expected op as <identifier> " + get_src_loc().str());
    }
    auto op = get_lexeme();
    std::vector<token> operands;
    next();
    while (!expect(token_type::comment) && !expect(token_type::newline)) {
        operands.push_back(lookahead);
        next();
    }
    std::vector<std::string> comments;
    consume_newline_comment(comments);

    for (auto &&comment: comments) {
        s.insert_comments(std::move(comment));
    }
    instruction instr(std::move(op), std::move(operands));
    s.insert_instr(std::move(instr));
}

void parser::parse_label(section &s) {
    if (!expect(token_type::label)) {
        throw std::runtime_error("label: expected <label> " + get_src_loc().str());
    }
    auto label = get_lexeme();
    next();

    std::vector<std::string> comments;
    consume_newline_comment(comments);
    for (auto &&comment: comments) {
        s.insert_comments(std::move(comment));
    }

    s.insert_label(label);
}

}
