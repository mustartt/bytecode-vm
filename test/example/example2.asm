module init_test

section text
_start:
    const_i32 $0
    const_i32 $1
    add_i32
    halt

section data

section readonly
    const 0 i32 5
    const 1 i32 10
