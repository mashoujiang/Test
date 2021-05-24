cmake_minimum_required(VERSION 3.10)

project(CppConcurrency)

add_executable(Chapter2 Chapter2.cpp Chapter2.hpp)
target_link_libraries(Chapter2 pthread)