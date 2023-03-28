cmake_minimum_required(VERSION 3.24)
project(MallocReplacement)

set(CMAKE_CXX_STANDARD 17)

add_executable(MallocReplacement main.cpp MyMalloc.cpp MyMalloc.hpp HashTable.cpp HashTable.hpp)
