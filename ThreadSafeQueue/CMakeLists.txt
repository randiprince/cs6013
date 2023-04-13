cmake_minimum_required(VERSION 3.24)
project(ThreadSafeQueue)

set(CMAKE_CXX_STANDARD 17)

add_executable(ThreadSafeQueue SerialQueue.hpp SerialQueueTests.cpp)

add_executable(ConcurrentQueue ConcurrentQueue.hpp ConcurrentQueueTest.cpp)
