#include <iostream>
#include <thread>
#include <vector>
#include "gtest/gtest.h"

class CppFeature: public ::testing::Test{

};

class Base
{
public:
    Base() = default;
    ~Base() = default;

private:
    int data{0};
};

TEST_F(CppFeature, PlacementNew){
    char buf[10000 * sizeof(Base)];
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i)
    {
        Base *base = new (buf) Base();
        base->~Base();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto placementNewDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i)
    {
        Base *base = new Base();
        delete base;
    }
    t2 = std::chrono::high_resolution_clock::now();
    auto newDuration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

    ASSERT_LT(placementNewDuration, newDuration);
}
