//
// Created by henry on 12/18/2023.
//

#ifndef BYTECODE_VM_INCLUDE_COMMON_H_
#define BYTECODE_VM_INCLUDE_COMMON_H_

namespace vm {

class Bytecode {
  public:
    enum Opcode : uint8_t {
        halt = 0x01,

        const_i32 = 0x02, // const_i32(uint16_t offset)
        add_i32 = 0x03,
        sub_i32 = 0x04,
        mul_i32 = 0x05,
        div_i32 = 0x06,
        rem_i32 = 0x07,
    };
};

}

#endif //BYTECODE_VM_INCLUDE_COMMON_H_
