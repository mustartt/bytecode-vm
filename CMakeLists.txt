cmake_minimum_required(VERSION 3.20)
project(bytecode_vm)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.74 COMPONENTS fiber log REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fsanitize=address,undefined -fno-omit-frame-pointer")
#set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 --coverage")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

# target assembler
add_library(
        assembly
        include/assembler/assembler.h
        include/assembler/tokenizer.h
        src/assembler/tokenizer.cpp
        include/assembler/parser.h
        src/assembler/parser.cpp
        include/assembler/src_loc.h
        src/assembler/src_loc.cpp
        include/assembler/token.h
        src/assembler/token.cpp
        src/assembler/assembler.cpp
        src/assembler/bytecode_format.cpp
        include/assembler/bytecode_compiler.h
        src/assembler/bytecode_compiler.cpp
        include/assembler/bytecode_format.h
        src/assembler/assembler_instr.cpp
)
target_link_libraries(assembly PUBLIC ${Boost_LIBRARIES})

add_executable(
        assembler
        src/assembler/assembler_main.cpp
)
target_link_libraries(assembler PUBLIC ${Boost_LIBRARIES})
target_link_libraries(assembler PUBLIC assembly)


# target bytecode_vm
add_executable(
        bytecode_vm
        src/main.cpp
        src/vm/vm_context.cpp
        include/vm/virtual_machine.h
        include/common/common.h
        include/vm/vm_proc.h
        include/vm/vm_proc.h
        src/vm/virtual_machine.cpp
        include/vm/shared_object.h
        include/vm/bytecode_linker.h
        include/vm/memory.h
)
target_link_libraries(bytecode_vm PUBLIC ${Boost_LIBRARIES})
target_link_libraries(bytecode_vm PUBLIC assembly)

# target main tests
enable_testing()
add_executable(
        main_test
        test/test_main.cpp
        test/common/shared_object_test.cpp
        test/assembler/src_loc_test.cpp
        test/assembler/bytecode_format_test.cpp
        test/assembler/assembler_test.cpp
)
target_link_libraries(main_test GTest::gtest_main)
target_link_libraries(main_test assembly)
gtest_discover_tests(main_test)
