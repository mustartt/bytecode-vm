
#include <fstream>
#include <iostream>

#include "assembler/assembler.h"

using namespace vm;

int main() {
    std::ifstream test_file("../include/assembler/test.asm");
    tokenizer token_stream(test_file);

    std::optional<tokenizer::token> tok;
    do {
        tok = token_stream.next();
        if (tok) {
            auto&[type, lexeme, loc] = tok.value();
            std::cout << loc << " " << static_cast<int>(type.get()) << " [" << lexeme << "]" << std::endl;
        }
    } while (tok);

    return 0;
}
