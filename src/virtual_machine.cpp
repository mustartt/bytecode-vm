//
// Created by henry on 12/19/2023.
//

#include <boost/fiber/fiber.hpp>

#include "vm_proc.h"
#include "virtual_machine.h"

namespace vm {

int virtual_machine::start_execution() {
    int exit_code = 0;
    ctx.start_parent([&exit_code, this] {
      vm_proc proc(*this, &program_data[0]);
      exit_code = proc.eval_loop();
    });
    return exit_code;
}

int vm_proc::eval_loop() {
    BOOST_LOG_TRIVIAL(trace) << "fiber: " << boost::this_fiber::get_id()
                             << " eval_loop() started";
    while (true) {
        uint8_t opcode = read_instr_uint8_t();
        switch (opcode) {
            case Bytecode::halt: {
                auto exit_code = std::bit_cast<int32_t>(pop_uint32_t());
                BOOST_LOG_TRIVIAL(trace) << "fiber: " << boost::this_fiber::get_id()
                                         << " halted with (ec: " << exit_code << ").";
                return exit_code;
            }
            case Bytecode::const_i32: {
                auto offset = read_instr_uint16_t();
                auto value = vm.read_const_uint32_t(offset);
                push(value);
                break;
            }
            case Bytecode::add_i32: {
                auto op2 = std::bit_cast<int32_t>(pop_uint32_t());
                auto op1 = std::bit_cast<int32_t>(pop_uint32_t());
                push(std::bit_cast<uint32_t>(op1 + op2));
                break;
            }
            case Bytecode::sub_i32: {
                auto op2 = std::bit_cast<int32_t>(pop_uint32_t());
                auto op1 = std::bit_cast<int32_t>(pop_uint32_t());
                push(std::bit_cast<uint32_t>(op1 - op2));
                break;
            }
            case Bytecode::mul_i32: {
                auto op2 = std::bit_cast<int32_t>(pop_uint32_t());
                auto op1 = std::bit_cast<int32_t>(pop_uint32_t());
                push(std::bit_cast<uint32_t>(op1 * op2));
                break;
            }
            case Bytecode::div_i32: {
                auto op2 = std::bit_cast<int32_t>(pop_uint32_t());
                auto op1 = std::bit_cast<int32_t>(pop_uint32_t());
                push(std::bit_cast<uint32_t>(op1 / op2));
                break;
            }
            case Bytecode::rem_i32: {
                auto op2 = std::bit_cast<int32_t>(pop_uint32_t());
                auto op1 = std::bit_cast<int32_t>(pop_uint32_t());
                push(std::bit_cast<uint32_t>(op1 % op2));
                break;
            }
            case Bytecode::pop_i32: {
                pop_uint32_t();
                break;
            }
            case Bytecode::load_addr: {
                auto index = read_instr_uint16_t();
                auto addr_offset = &vm.data()[0] + vm.read_const_uint64_t(index);
                push(std::bit_cast<uint64_t>(addr_offset));
                break;
            }
            case Bytecode::load_rel_i32: {
                auto fp_offset = std::bit_cast<int16_t>(read_instr_uint16_t());
                auto load_addr = fp + fp_offset;
                int32_t value;
                std::memcpy(&value, load_addr, sizeof(value));
                push(std::bit_cast<uint32_t>(value));
                break;
            }
            case Bytecode::call: {
                auto arg_size = read_instr_uint8_t();
                auto jump_to = pop_uint64_t();
                push_frame();
                fp = sp - arg_size;
                pc = std::bit_cast<decltype(pc)>(jump_to);
                break;
            }
            case Bytecode::ret: {
                pop_frame();
                break;
            }
            default: {
                BOOST_LOG_TRIVIAL(trace) << "fiber: " << boost::this_fiber::get_id()
                                         << " invalid state";
                return -1;
            }
        }
    }
}

void vm_proc::push_frame() {
    stack_frame.emplace_back(fp, pc);
}

void vm_proc::pop_frame() {
    auto[old_fp, call_site] = stack_frame.back();
    fp = old_fp;
    pc = call_site;
    stack_frame.pop_back();
}

}
