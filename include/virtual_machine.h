//
// Created by henry on 12/18/2023.
//

#ifndef BYTECODE_VM_INCLUDE_VIRTUAL_MACHINE_H_
#define BYTECODE_VM_INCLUDE_VIRTUAL_MACHINE_H_

#include <atomic>

#include "common.h"
#include "vm_context.h"

#define METHOD_POP(type) \
    type pop_##type() { \
        type value; \
        sp -= sizeof(value); \
        std::memcpy(&value, sp, sizeof(value)); \
        return value; \
    } \

#define METHOD_PUSH(type) \
    void push(type value) { \
        std::memcpy(sp, &value, sizeof(value)); \
        sp += sizeof(value); \
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
        std::memcpy(&value, &constant_pool[0] + offset, sizeof(value)); \
        return value; \
    } \


namespace vm {

constexpr size_t STACK_SIZE = 2 << 10; // 2kb stack size

class vm_proc;

class virtual_machine {
  public:
    using memory = std::vector<uint8_t>;

    explicit virtual_machine(vm_context &ctx, memory const_pool, memory data)
        : program_data(std::move(data)), constant_pool(std::move(const_pool)), ctx(ctx) {}
  public:
    int start_execution();

  public:
    memory &data() { return program_data; }

    METHOD_CONST(uint8_t);
    METHOD_CONST(uint16_t);
    METHOD_CONST(uint32_t);
    METHOD_CONST(uint64_t);

  private:
    memory program_data;  // uses uint8_t* to read
    memory constant_pool; // uses uint16_t to offset into
  private:
    vm_context &ctx;
    std::atomic<bool> shutdown;
};



}

#endif //BYTECODE_VM_INCLUDE_VIRTUAL_MACHINE_H_
