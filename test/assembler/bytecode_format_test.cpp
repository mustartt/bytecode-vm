//
// Created by henry on 12/27/2023.
//
#include "gtest/gtest.h"

#include "assembler/assembler.h"  // Replace with the actual header file name

using namespace vm;

// Test fixture for bytecode_format tests
class BytecodeFormatTest : public ::testing::Test {
  protected:
    bytecode_format bytecode;

    void SetUp() override {}
};

TEST_F(BytecodeFormatTest, SerializationDeserialization) {
    bytecode.metadata = "TestMetadata";
    bytecode.dependency = {"Dependency1", "Dependency2"};
    bytecode.symbol_table["Symbol1"] = {true, 10, 20};
    bytecode.data = {1, 2, 3};
    bytecode.constpool = {4, 5, 6};
    bytecode.comments[0] = "Comment";

    std::ostringstream oss;
    bytecode.serialize(oss);

    bytecode_format deserializedBytecode;
    std::istringstream iss(oss.str());
    deserializedBytecode.deserialize(iss);

    EXPECT_EQ(bytecode.metadata, deserializedBytecode.metadata);
    EXPECT_EQ(bytecode.dependency, deserializedBytecode.dependency);
    EXPECT_EQ(bytecode.symbol_table, deserializedBytecode.symbol_table);
    EXPECT_EQ(bytecode.constpool, deserializedBytecode.constpool);
    EXPECT_EQ(bytecode.data, deserializedBytecode.data);
}
