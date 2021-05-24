#include "Chapter2.hpp"

int main()
{
    std::vector<int> nums(100);
    std::iota(nums.begin(), nums.end(), 1);
    std::cout << Chapter2::parallel_accumulate(nums.begin(), nums.end(), 0) << std::endl;
}