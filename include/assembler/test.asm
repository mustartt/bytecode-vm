module some_module

include "other.asm"

section text

_start:
    const_ptr $4    ; loads main([str])i32 onto call stack
    call            ; calls main function

    const_i32 $1    ; loads exit_code 1
    halt            ; halts thread
                    ; return exit_code

main([str])i32:
    const_str $0
    const_ptr $5
    call

    const_i32 $1
    ret


section data
    vtable_identifier       u16         $6 $7 $8 $9 ; offset into the const pool
    mutex_identifier        mutex
    condvar_identifier      condvar
    buf_chan_identifier     bufchan
    unbuf_chan_identifier   unbufchan

section readonly
    constpool 0 str "Hello World\n"
    constpool 1 i32 0
    constpool 2 i32 10
    constpool 3 i32 25
    constpool 4 addr main([str])i32
    constpool 5 addr println(str)
    constpool 6 addr dyn_func1
    constpool 7 addr dyn_func2
    constpool 8 addr dyn_func3
    constpool 9 addr dyn_func4


