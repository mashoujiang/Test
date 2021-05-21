cmake_minimum_required(VERSION 3.10)

project(CoreTest)

add_library(FirstPlugin FirstPlugin.cpp FirstPlugin.hpp)
add_library(SecondPlugin SecondPlugin.cpp SecondPlugin.hpp)

add_executable(CoreTest Core.cpp)
target_link_libraries(CoreTest PRIVATE FirstPlugin SecondPlugin)
