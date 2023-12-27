//
// Created by henry on 12/27/2023.
//

#include <gtest/gtest.h>

#include "assembler/src_loc.h"

using namespace vm;

TEST(SrcLocTest, DefaultConstructor) {
    src_loc loc;
    EXPECT_EQ(0u, loc.start_line);
    EXPECT_EQ(0u, loc.start_col);
    EXPECT_EQ(0u, loc.end_line);
    EXPECT_EQ(0u, loc.end_col);
}

TEST(SrcLocTest, ParameterizedConstructor) {
    src_loc loc(1, 2, 3, 4);
    EXPECT_EQ(1u, loc.start_line);
    EXPECT_EQ(2u, loc.start_col);
    EXPECT_EQ(3u, loc.end_line);
    EXPECT_EQ(4u, loc.end_col);
}

TEST(SrcLocTest, StrFunction) {
    src_loc loc(1, 2, 3, 4);
    EXPECT_EQ("(1:2,3:4)", loc.str());
}

TEST(SrcLocTest, OutputOperator) {
    src_loc loc(1, 2, 3, 4);
    std::ostringstream oss;
    oss << loc;
    EXPECT_EQ("(1:2,3:4)", oss.str());
}
