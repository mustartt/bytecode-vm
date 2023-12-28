//
// Created by henry on 12/27/2023.
//

#include "gtest/gtest.h"

#include "assembler/assembler.h"
#include "common.h"

using namespace vm;

class AssemblerTest : public ::testing::Test {};

TEST_F(AssemblerTest, EmitSingleInstr) {
    assembler assembler;

    assembler.emit_halt();

    auto &bytecode = assembler.get_bytecode().data;
    EXPECT_EQ(bytecode.size(), 1);
    EXPECT_EQ(bytecode[0], Bytecode::halt);
}

TEST_F(AssemblerTest, EmitMultipleInstr) {
    assembler assembler;

    assembler.emit_const_i32(1);
    assembler.emit_const_i32(2);
    assembler.emit_add_i32();

    std::vector<uint8_t> expected{
        Bytecode::const_i32, 0x01, 0x00,
        Bytecode::const_i32, 0x02, 0x00,
        Bytecode::add_i32,
    };

    auto &bytecode = assembler.get_bytecode().data;
    EXPECT_EQ(bytecode, expected);
}

TEST_F(AssemblerTest, EmitLabelOffset) {
    assembler assembler;

    assembler.register_label("label1");
    assembler.emit_const_i32(1);
    assembler.register_label("label2");
    assembler.register_label("label3");
    assembler.emit_const_i32(2);
    assembler.register_label("label4");
    assembler.emit_add_i32();
    assembler.register_label("label5");

    auto &table = assembler.get_bytecode().symbol_table;
    EXPECT_NO_THROW(table.at("label1"));
    EXPECT_NO_THROW(table.at("label2"));
    EXPECT_NO_THROW(table.at("label3"));
    EXPECT_NO_THROW(table.at("label4"));
    EXPECT_NO_THROW(table.at("label5"));
    EXPECT_ANY_THROW(table.at("label_other"));

    EXPECT_EQ(table.at("label1").resolved, true);
    EXPECT_EQ(table.at("label1").data_offset, 0);

    EXPECT_EQ(table.at("label2").resolved, true);
    EXPECT_EQ(table.at("label2").data_offset, 3);

    EXPECT_EQ(table.at("label3").resolved, true);
    EXPECT_EQ(table.at("label3").data_offset, 3);

    EXPECT_EQ(table.at("label4").resolved, true);
    EXPECT_EQ(table.at("label4").data_offset, 6);

    EXPECT_EQ(table.at("label5").resolved, true);
    EXPECT_EQ(table.at("label5").data_offset, 7);
}

TEST_F(AssemblerTest, VariableAlignmentSmall) {
    assembler assembler;

    assembler.reserve_aligned_data_var("var1", 1);
    assembler.reserve_aligned_data_var("var2", 2);

    auto &bytecode = assembler.get_bytecode().data;
    EXPECT_EQ(bytecode.size(), 8 + 2);

    auto &table = assembler.get_bytecode().symbol_table;
    EXPECT_NO_THROW(table.at("var1"));
    EXPECT_NO_THROW(table.at("var2"));

    EXPECT_EQ(table.at("var1").resolved, true);
    EXPECT_EQ(table.at("var1").data_offset, 0);

    EXPECT_EQ(table.at("var2").resolved, true);
    EXPECT_EQ(table.at("var2").data_offset, 8);
}

TEST_F(AssemblerTest, VariableAlignmentLarge) {
    assembler assembler;

    assembler.reserve_aligned_data_var("var1", 8);
    assembler.reserve_aligned_data_var("var2", 13);
    assembler.reserve_aligned_data_var("var3", 8);

    auto &bytecode = assembler.get_bytecode().data;
    EXPECT_EQ(bytecode.size(), 32);

    auto &table = assembler.get_bytecode().symbol_table;
    EXPECT_NO_THROW(table.at("var1"));
    EXPECT_NO_THROW(table.at("var2"));
    EXPECT_NO_THROW(table.at("var3"));

    EXPECT_EQ(table.at("var1").resolved, true);
    EXPECT_EQ(table.at("var1").data_offset, 0);

    EXPECT_EQ(table.at("var2").resolved, true);
    EXPECT_EQ(table.at("var2").data_offset, 8);

    EXPECT_EQ(table.at("var3").resolved, true);
    EXPECT_EQ(table.at("var3").data_offset, 24);
}

TEST_F(AssemblerTest, WriteConsti32) {
    assembler assembler;

    assembler.write_const_i32(0, 123);
    assembler.write_const_i32(5, 456);
    assembler.write_const_i32(3, -78);

    auto &pool = assembler.get_bytecode().constpool;
    EXPECT_EQ(pool.size(), 6);
    EXPECT_EQ(pool[0], 123);
    EXPECT_EQ(pool[5], 456);
    EXPECT_EQ(pool[3], -78);
}

TEST_F(AssemblerTest, WriteConstStr) {
    assembler assembler;

    std::string const_str = "Hello World";
    assembler.write_const_str(0, const_str);

    auto &pool = assembler.get_bytecode().constpool;
    EXPECT_EQ(pool.size(), 1);
    EXPECT_EQ(pool[0], 0);

    auto &data = assembler.get_bytecode().data;
    EXPECT_EQ(data.size(), const_str.size() + 1);
    EXPECT_EQ(data[0], 'H');
    EXPECT_EQ(data[4], 'o');
    EXPECT_EQ(data[10], 'd');
    EXPECT_EQ(data[const_str.size()], '\0');
}

TEST_F(AssemblerTest, WriteConstAddrSymbolResolved) {
    assembler assembler;

    assembler.emit_const_i32(1);
    assembler.emit_const_i32(2);
    assembler.register_label("label");      // 6 byte offset
    assembler.emit_add_i32();

    assembler.write_const_addr(0, "label");

    auto &pool = assembler.get_bytecode().constpool;
    EXPECT_EQ(pool.size(), 1);
    EXPECT_EQ(pool[0], 6);
}

TEST_F(AssemblerTest, WriteConstAddrSymbolUnresolved) {
    assembler assembler;

    assembler.emit_const_i32(1);
    assembler.emit_const_i32(2);
    assembler.emit_add_i32();

    assembler.write_const_addr(1, "unresolved");
    auto &pool = assembler.get_bytecode().constpool;
    EXPECT_EQ(pool.size(), 2);
    EXPECT_EQ(pool[1], 0);

    auto &table = assembler.get_bytecode().symbol_table;
    EXPECT_NO_THROW(table.at("unresolved"));
    EXPECT_EQ(table.at("unresolved").resolved, false);
    EXPECT_EQ(table.at("unresolved").const_offset, 1);

    assembler.register_label("unresolved");
    EXPECT_EQ(table.at("unresolved").resolved, true);
    EXPECT_EQ(table.at("unresolved").const_offset, 1);
    EXPECT_EQ(table.at("unresolved").data_offset, 7);

    EXPECT_EQ(pool[1], 7);
}

