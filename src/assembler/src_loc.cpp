//
// Created by henry on 12/27/2023.
//

#include "assembler/src_loc.h"

namespace vm {

src_loc::src_loc(size_t start_line, size_t start_col, size_t end_line, size_t end_col)
    : start_line(start_line), start_col(start_col),
      end_line(end_line), end_col(end_col) {}

std::string src_loc::str() const {
    std::ostringstream os;
    os << *this;
    return os.str();
}

std::ostream &operator<<(std::ostream &os, const src_loc &loc) {
    os << "(" << loc.start_line << ":" << loc.start_col << "," << loc.end_line
       << ":" << loc.end_col << ")";
    return os;
}

}
