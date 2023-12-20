//
// Created by henry on 12/19/2023.
//

#ifndef BYTECODE_VM_INCLUDE_VM_PROC_H_
#define BYTECODE_VM_INCLUDE_VM_PROC_H_

#include "virtual_machine.h"

namespace vm {

class vm_proc {
  public:
    using frame_ptr = uint8_t *;
    using call_site = uint8_t *;
    using frame = std::pair<frame_ptr, call_site>;

    vm_proc(virtual_machine &vm, uint8_t *start) : vm(vm), pc(start) {
        stack.resize(STACK_SIZE, 0);
        fp = &stack[0];
        sp = &stack[0];
        stack_frame.emplace_back(fp, pc);
    }
  public:
    int eval_loop();

    METHOD_PUSH(uint8_t);
    METHOD_PUSH(uint16_t);
    METHOD_PUSH(uint32_t);
    METHOD_PUSH(uint64_t);

    METHOD_POP(uint8_t);
    METHOD_POP(uint16_t);
    METHOD_POP(uint32_t);
    METHOD_POP(uint64_t);

    METHOD_INSTR(uint8_t);
    METHOD_INSTR(uint16_t);
    METHOD_INSTR(uint32_t);
    METHOD_INSTR(uint64_t);

    void push_frame();
    void pop_frame();

  private:
    virtual_machine &vm;
    virtual_machine::memory stack;
    std::vector<frame> stack_frame;
  private:
    uint8_t *pc;
    uint8_t *fp;
    uint8_t *sp;
};

}

#endif //BYTECODE_VM_INCLUDE_VM_PROC_H_
