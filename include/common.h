//
// Created by henry on 12/18/2023.
//

#ifndef BYTECODE_VM_INCLUDE_COMMON_H_
#define BYTECODE_VM_INCLUDE_COMMON_H_

#include <cstdint>

namespace vm {

// ===============================================
// load_<type>          load at address
// load_offset_<type>   load at address + offset
// load_array_<type>    load at array + index
// load_local_<type>    load relative to fp
// store_<type>         store at address
// store_offset_<type>  store at address + offset
// store_array_<type>   store at array + index
// store_local_<type>   store relative to fp
// ===============================================

/**
 * Core Data Types
 *  Numbers: i8, i16, i32, i64, f32, f64
 *  Reference: Shared Reference and Array
 *  Sync: mutex, condvar, bufchan, unbufchan, barrier
 */
class Bytecode {
  public:
    enum Opcode : uint8_t {
        halt = 0x00,
        nop = 0x01,

        const_i8,
        const_i16,

        const_i32, // const_i32(uint16_t offset)
        add_i32,
        sub_i32,
        mul_i32,
        div_i32,
        rem_i32,
        pop_i32,

        load_i32,
        load_offset_i32,
        load_array_i32,
        load_local_i32,
        store_i32,
        store_offset_i32,
        store_array_i32,
        store_local_i32,

        const_i64,
        add_i64,
        sub_i64,
        mul_i64,
        div_i64,
        rem_i64,
        pop_i64,

        load_i64,
        load_offset_i64,
        load_array_i64,
        load_local_i64,
        store_i64,
        store_offset_i64,
        store_array_i64,
        store_local_i64,

        const_f32,
        add_f32,
        sub_f32,
        mul_f32,
        div_f32,
        rem_f32,
        pop_f32,

        load_f32,
        load_offset_f32,
        load_array_f32,
        load_local_f32,
        store_f32,
        store_offset_f32,
        store_array_f32,
        store_local_f32,

        const_f64,
        add_f64,
        sub_f64,
        mul_f64,
        div_f64,
        rem_f64,
        pop_f64,

        load_f64,
        load_offset_f64,
        load_array_f64,
        load_local_f64,
        store_f64,
        store_offset_f64,
        store_array_f64,
        store_local_f64,

        /** @brief type conversion between small integer types
         */
        convert_i8_i32,
        convert_i16_i32,
        convert_i32_i8,
        convert_i32_i16,
        /* conversions between int and floating types */
        convert_i32_i64,
        convert_i32_f32,
        convert_i32_f64,
        convert_i64_f32,
        convert_i64_f64,
        convert_f32_f64,
        convert_i64_i32,
        convert_f32_i32,
        convert_f32_i64,
        convert_f64_i32,
        convert_f64_i64,
        convert_f64_f32,

        cmp_i32,
        cmp_i64,
        cmp_f32,
        cmp_f64,
        cmp_ref,
        cmp_array,
        cmp_ref_null,
        cmp_array_null,

        new_ref,
        acquire_ref,
        release_ref,
        finalize_ref,
        load_ref,
        load_offset_ref,
        load_array_ref,
        load_local_ref,
        store_ref,
        store_offset_ref,
        store_array_ref,
        store_local_ref,

        new_array,
        acquire_array,
        release_array,
        finalize_array,
        load_array,
        load_offset_array,
        load_array_array,
        load_local_array,
        store_array,
        store_offset_array,
        store_array_array,
        store_local_array,

        init_mutex,
        destroy_mutex,
        lock_mutex,
        unlock_mutex,
        load_mutex,
        load_offset_mutex,
        load_array_mutex,
        load_local_mutex,

        init_condvar,
        destroy_condvar,
        notify_all_condvar,
        notify_one_condvar,
        wait_condvar,
        load_condvar,
        load_offset_condvar,
        load_array_condvar,
        load_local_condvar,

        init_bufchan,
        close_bufchan,
        destroy_bufchan,
        push_bufchan,
        poll_bufchan,
        load_bufchan,
        load_offset_bufchan,
        load_array_bufchan,
        load_local_bufchan,

        init_unbufchan,
        close_unbufchan,
        destroy_unbufchan,
        push_unbufchan,
        poll_unbufchan,
        load_unbufchan,
        load_offset_unbufchan,
        load_array_unbufchan,
        load_local_unbufchan,

        /**
         * @brief @code call $arg_size @endcode
         *
         * Stack Effect: arg1, arg2, ..., func_addr -> res1, ...
         *
         * @param arg_size  the size (in bytes) of the arguments passed to the called function
         * @param fund_addr address of the function called
         */
        call,
        call_native,
        ret,
        branch,

        load_addr,

        if_eq0_i32,
        if_ne0_i32,
        if_lt0_i32,
        if_ge0_i32,
        if_gte0_i32,
        if_lte0_i32,

        if_cmp_eq_i32,
        if_cmp_ne_i32,
        if_cmp_lt_i32,
        if_cmp_gt_i32,
        if_cmp_lte_i32,
        if_cmp_gte_i32,
    };
};

}

#endif //BYTECODE_VM_INCLUDE_COMMON_H_
