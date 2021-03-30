#include <iostream>
#include <thread>

class Test
{
public:
    Test() = default;
    ~Test() = default;

private:
    int data{0};
};

int main()
{
    char buf[10000 * sizeof(Test)];
    auto t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i)
    {
        Test *test = new (buf) Test();
        test->~Test();
    }
    auto t2 = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "placement new: " << duration << std::endl;

    t1 = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 10000; ++i)
    {
        Test *test = new Test();
        delete test;
    }
    t2 = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
    std::cout << "new: " << duration << std::endl;
}