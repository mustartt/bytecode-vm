//
// Created by henry on 12/28/2023.
//

#ifndef BYTECODE_VM_INCLUDE_VM_BYTECODE_LINKER_H_
#define BYTECODE_VM_INCLUDE_VM_BYTECODE_LINKER_H_

#include "memory.h"
#include "assembler/bytecode_format.h"

namespace vm {

class bytecode_linker {
  public:
    bytecode_linker(cpool &const_pool) : const_pool(const_pool) {}

  public:
    memory load_module(const bytecode_format &bytecode) {
        if (std::find(loaded_module.begin(), loaded_module.end(),
                      bytecode.module_name()) != loaded_module.end()) {
            throw std::runtime_error("bytecode_linker: module already loaded " + bytecode.module_name());
        }

        // todo: redesign bytecode format and symbol table
        loaded_module.push_back(bytecode.module_name());
        memory new_page(bytecode.data.size());
        std::memcpy(new_page.data(), bytecode.data.data(), bytecode.data.size());

        // todo: offset constants
        const_pool = bytecode.constpool;

        return new_page;
    }

  private:
    std::vector<std::string> loaded_module;
    std::map<std::string, symbol_entry> symbol_table;
    cpool &const_pool;
    // todo: add comment table
};

}

#endif //BYTECODE_VM_INCLUDE_VM_BYTECODE_LINKER_H_
