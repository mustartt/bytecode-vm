//
// Created by henry on 12/27/2023.
//

#include "assembler/assembler.h"

namespace vm {

void write_string(std::ostream &os, const std::string &str) {
    uint64_t size = str.size();
    os.write(reinterpret_cast<const char *>(&size), sizeof(uint64_t));
    os.write(str.data(), size);
}

void read_string(std::istream &is, std::string &str) {
    uint64_t size;
    is.read(reinterpret_cast<char *>(&size), sizeof(uint64_t));
    str.resize(size);
    is.read(&str[0], size);
}

void bytecode_format::serialize(std::ostream &os) {
    // Serialize metadata
    write_string(os, metadata);

    // Serialize dependency
    uint64_t dependency_size = dependency.size();
    os.write(reinterpret_cast<const char *>(&dependency_size), sizeof(uint64_t));
    for (const auto &dep: dependency) {
        write_string(os, dep);
    }

    // Serialize symbol_table
    uint64_t symbol_table_size = symbol_table.size();
    os.write(reinterpret_cast<const char *>(&symbol_table_size), sizeof(uint64_t));
    for (const auto &entry: symbol_table) {
        write_string(os, entry.first);
        os.write(reinterpret_cast<const char *>(&entry.second), sizeof(symbol_entry));
    }

    // Serialize data
    uint64_t data_size = data.size();
    os.write(reinterpret_cast<const char *>(&data_size), sizeof(uint64_t));
    os.write(reinterpret_cast<const char *>(data.data()), data_size);

    // Serialize constpool
    uint64_t const_pool_size = constpool.size();
    os.write(reinterpret_cast<const char *>(&const_pool_size), sizeof(uint64_t));
    os.write(reinterpret_cast<const char *>(constpool.data()), sizeof(uint64_t) * const_pool_size);

    // Serialize comments
    uint64_t comment_pool_size = comments.size();
    os.write(reinterpret_cast<const char *>(&comment_pool_size), sizeof(uint64_t));
    for (const auto &entry: comments) {
        os.write(reinterpret_cast<const char *>(&entry.first), sizeof(std::size_t));
        write_string(os, entry.second);
    }
}

void bytecode_format::deserialize(std::istream &is) {
    // Deserialize metadata
    read_string(is, metadata);

    // Deserialize dependency
    uint64_t dep_size;
    is.read(reinterpret_cast<char *>(&dep_size), sizeof(uint64_t));
    dependency.resize(dep_size);
    for (auto &dep: dependency) {
        read_string(is, dep);
    }

    // Deserialize symbol_table
    uint64_t symbol_table_size;
    is.read(reinterpret_cast<char *>(&symbol_table_size), sizeof(uint64_t));
    symbol_table.clear();
    for (uint64_t i = 0; i < symbol_table_size; ++i) {
        std::string key;
        symbol_entry value;
        read_string(is, key);
        is.read(reinterpret_cast<char *>(&value), sizeof(symbol_entry));
        symbol_table[key] = value;
    }

    // Deserialize data
    uint64_t data_size;
    is.read(reinterpret_cast<char *>(&data_size), sizeof(uint64_t));
    data.resize(data_size);
    is.read(reinterpret_cast<char *>(data.data()), data_size);

    // Deserialize constpool
    uint64_t const_pool_size;
    is.read(reinterpret_cast<char *>(&const_pool_size), sizeof(uint64_t));
    constpool.resize(const_pool_size);
    is.read(reinterpret_cast<char *>(constpool.data()), sizeof(uint64_t) * const_pool_size);

    // Deserialize comments
    uint64_t comment_pool_size;
    is.read(reinterpret_cast<char *>(&comment_pool_size), sizeof(uint64_t));
    comments.clear();
    for (uint64_t i = 0; i < comment_pool_size; ++i) {
        std::size_t key;
        std::string value;
        is.read(reinterpret_cast<char *>(&key), sizeof(std::size_t));
        read_string(is, value);
        comments[key] = value;
    }
}

}
