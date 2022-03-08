cmake_minimum_required(VERSION 3.16)
project(cpp_extension_method)

set(CMAKE_CXX_STANDARD 17)

add_executable(cpp_extension_method main.cpp mixin.h)
