//
// Created by henry on 12/18/2023.
//

#ifndef BYTECODE_VM_INCLUDE_VIRTUAL_MACHINE_H_
#define BYTECODE_VM_INCLUDE_VIRTUAL_MACHINE_H_

#include <atomic>

#include "common/common.h"
#include "memory.h"
#include "vm_context.h"
#include "bytecode_linker.h"

#define METHOD_POP(type) \
    type pop_##type() { \
        type value; \
        sp -= std::max(ALIGNMENT, sizeof(value)); \
        std::memcpy(&value, sp, sizeof(value)); \
        return value; \
    } \

#define METHOD_PUSH(type) \
    void push(type value) { \
        std::memcpy(sp, &value, sizeof(value)); \
        sp += std::max(ALIGNMENT, sizeof(value)); \
    } \

#define METHOD_INSTR(type) \
    type read_instr_##type() { \
        type value; \
        std::memcpy(&value, pc, sizeof(value)); \
        pc += sizeof(value); \
        return value; \
    } \

#define METHOD_CONST(type) \
    type read_const_##type(uint16_t offset) { \
        type value; \
        std::memcpy(&value, &constant_pool[offset], sizeof(value)); \
        return value; \
    } \


namespace vm {

constexpr size_t STACK_SIZE = 2 << 10; // 2kb stack size
constexpr size_t ALIGNMENT = 4;

class vm_proc;

class virtual_machine {
  public:
    explicit virtual_machine(vm_context &ctx)
        : program_data(), constant_pool(),
          ctx(ctx), linker(constant_pool) {}
  public:
    /** @brief starts execution from the first byte of the first loaded module
     */
    int start_execution();

  public:
    void load_module(const bytecode_format &bytecode) {
        // todo: aquire global exclusive lock
        program_data.push_back(linker.load_module(bytecode));
    }

    METHOD_CONST(uint8_t);
    METHOD_CONST(uint16_t);
    METHOD_CONST(uint32_t);
    METHOD_CONST(uint64_t);

  private:
    std::vector<memory> program_data;  // uses uint8_t* to read
    cpool constant_pool; // uses uint16_t to offset into

    vm_context &ctx;
    bytecode_linker linker;
    std::atomic<bool> shutdown;
};

}

#endif //BYTECODE_VM_INCLUDE_VIRTUAL_MACHINE_H_
