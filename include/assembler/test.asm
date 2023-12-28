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
    reserve vtable_identifier       u16         $6 $7 $8 ; offset into the const pool
    reserve mutex_identifier        mutex
    reserve condvar_identifier      condvar
    reserve buf_chan_identifier     bufchan
    reserve unbuf_chan_identifier   unbufchan

section readonly
    const 0 str "Hello World\n"
    const 1 i32 0
    const 2 i32 10
    const 3 i32 25
    const 4 addr main([str])i32
    const 5 addr println(str)
    const 6 addr dyn_func1
    const 7 addr dyn_func2
    const 8 addr dyn_func3
    const 9 addr other_func     ; unresolved function outside this module



