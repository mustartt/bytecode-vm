//
// Created by henry on 12/18/2023.
//

#ifndef BYTECODE_VM_INCLUDE_COMMON_H_
#define BYTECODE_VM_INCLUDE_COMMON_H_

#include <cstdint>

namespace vm {

/**
 * Core Data Types
 *  Numbers: i8, i16, i32, i64, f32, f64
 *  Reference: Shared Reference and Array
 *  Sync: mutex, condvar, bufchan, unbufchan, barrier
 */
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
        pop_i32 = 0x08,

        if_cmp_eq_i32,
        if_cmp_ne_i32,
        if_cmp_lt_i32,
        if_cmp_gt_i32,
        if_cmp_lte_i32,
        if_cmp_gte_i32,

        /**
         * \brief \code call $arg_size \endcode
         *
         * Stack Effect: arg1, arg2, ..., func_addr -> res1, ...
         *
         * @param arg_size  the size (in bytes) of the arguments passed to the called function
         * @param fund_addr address of the function called
         */
        call = 0x09,
        call_native,

        ret = 0x0,
        load_addr = 0x0b,
        load_rel_i32 = 0x0c,

    };
};

}

#endif //BYTECODE_VM_INCLUDE_COMMON_H_
