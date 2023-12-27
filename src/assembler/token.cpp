//
// Created by henry on 12/27/2023.
//

#include <boost/assert.hpp>

#include "assembler/token.h"

namespace vm {

std::string token_type::str() const {
    switch (t) {
        case eof: return "eof";
        case whitespace: return "ws";
        case comment: return "comment";
        case label:return "label";
        case identifier:return "ident";
        case numeric:return "numeric";
        case offset:return "offset";
        case newline:return "newline";
        case string:return "string";
        case section:return "section";
        case include:return "include";
    }
    BOOST_ASSERT_MSG(false, "invalid state");
}

}
