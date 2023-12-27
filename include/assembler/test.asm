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
    offset 0 str "Hello World\n"
    offset 1 i32 0
    offset 2 i32 10
    offset 3 i32 25
    offset 4 addr main([str])i32
    offset 5 addr println(str)
    offset 6 addr dyn_func1
    offset 7 addr dyn_func2
    offset 8 addr dyn_func3
    offset 9 addr dyn_func4


