//
// Created by henry on 12/27/2023.
//

#ifndef BYTECODE_VM_SRC_ASSEMBLER_SRC_LOC_H_
#define BYTECODE_VM_SRC_ASSEMBLER_SRC_LOC_H_

#include <string>
#include <sstream>

namespace vm {

class src_loc {
  public:
    constexpr src_loc() noexcept: start_line(0), start_col(0),
                                  end_line(0), end_col(0) {}
    src_loc(size_t start_line, size_t start_col,
            size_t end_line, size_t end_col);
    [[nodiscard]] std::string str() const;
    friend std::ostream &operator<<(std::ostream &os, const src_loc &loc);
  public:
    size_t start_line, start_col;
    size_t end_line, end_col;
};

}

#endif //BYTECODE_VM_SRC_ASSEMBLER_SRC_LOC_H_
