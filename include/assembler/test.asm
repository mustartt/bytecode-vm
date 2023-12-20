section .text

_start:
    const_ptr #4
    call
    halt

main([str])i32:
    const_str #0
    const_ptr #5
    call

    const_i32 #1
    ret


section .data
    vtable_identifier       u16         6, 7, 8, 9 ; offset into the const pool
    mutex_identifier        mutex
    condvar_identifier      condvar
    buf_chan_identifier     bufchan
    unbuf_chan_identifier   unbufchan

section .ro
    0 str 'Hello World\n'
    1 i32 0
    2 i32 10
    3 i32 25
    4 addr main([str])i32
    5 addr println(str)
    6 addr dyn_func1
    7 addr dyn_func2
    8 addr dyn_func3
    9 addr dyn_func4


