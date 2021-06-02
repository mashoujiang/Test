cmake_minimum_required(VERSION 3.10)

project(TestApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(thirdparty)

add_executable(CppFeature CppFeature.cpp)
target_link_directories(CppFeature PRIVATE ${GTEST_INCLUDE_DIRS})
target_link_libraries(CppFeature PRIVATE gtest gmock gtest_main pthread atomic)

add_subdirectory(C++_ConCurrency_In_Action)