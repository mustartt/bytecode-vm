cmake_minimum_required(VERSION 3.22)
project(bytecode_vm)
set(CMAKE_CXX_STANDARD 20)

find_package(Boost 1.74 COMPONENTS fiber log REQUIRED)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -fsanitize=address -fno-omit-frame-pointer")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

include_directories(include)
include_directories(${Boost_INCLUDE_DIRS})

add_executable(
        bytecode_vm
        src/main.cpp
        src/vm_context.cpp
        include/virtual_machine.h
        include/common.h
        include/vm_proc.h
        include/vm_proc.h
        src/virtual_machine.cpp
)
target_link_libraries(bytecode_vm ${Boost_LIBRARIES})
