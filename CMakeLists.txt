cmake_minimum_required(VERSION 3.22)
project(bytecode_vm)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.74 COMPONENTS fiber log REQUIRED)
find_package(GTest REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(
        bytecode_vm
        src/main.cpp
        src/vm_context.cpp
        include/virtual_machine.h
        include/common.h
        include/vm_proc.h
        include/vm_proc.h
        src/virtual_machine.cpp
        include/common/shared_object.h
)
target_link_libraries(bytecode_vm ${Boost_LIBRARIES})

enable_testing()
add_executable(
        main_test
        test/test_main.cpp
        test/common/shared_object_test.cpp
)
target_link_libraries(main_test GTest::gtest_main)
gtest_discover_tests(main_test)
