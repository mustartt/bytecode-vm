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
        proc.eval_loop();
        exit_code = std::bit_cast<int32_t>(proc.pop_uint32_t());
    });
    return exit_code;
}

void vm_proc::eval_loop() {
    BOOST_LOG_TRIVIAL(trace) << "fiber: " << boost::this_fiber::get_id()
                             << " eval_loop() started";
    while (true) {
        uint8_t opcode = read_instr_uint8_t();
        switch (opcode) {
            case Bytecode::halt: {
                auto exit_code = pop_uint32_t();
                BOOST_LOG_TRIVIAL(trace) << "fiber: " << boost::this_fiber::get_id()
                                         << " halted with (ec: " << exit_code << ").";
                return;
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
            default: {
                BOOST_LOG_TRIVIAL(trace) << "fiber: " << boost::this_fiber::get_id()
                                         << " invalid state";
                return;
            }
        }
    }
}

}
