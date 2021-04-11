#include "gtest/gtest.h"
#include <climits>
#include <iostream>
#include <numeric>
#include <queue>
#include <thread>
#include <vector>

class CppFeature : public ::testing::Test {};

class Base {
public:
  Base() = default;
  ~Base() = default;

private:
  int data{0};
};

TEST_F(CppFeature, PlacementNew) {
  char buf[10000 * sizeof(Base)];
  auto t1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 10000; ++i) {
    Base *base = new (buf) Base();
    base->~Base();
  }
  auto t2 = std::chrono::high_resolution_clock::now();
  auto placementNewDuration =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  t1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < 10000; ++i) {
    Base *base = new Base();
    delete base;
  }
  t2 = std::chrono::high_resolution_clock::now();
  auto newDuration =
      std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();

  ASSERT_LT(placementNewDuration, newDuration);
}

TEST_F(CppFeature, PriorityQueue) {
  std::priority_queue<int> mypq;

  mypq.push(30);
  mypq.push(100);
  mypq.push(25);
  mypq.push(40);

  int last = INT_MAX;
  while (!mypq.empty()) {
    int top = mypq.top();
    ASSERT_GE(last, top);
    last = top;
    mypq.pop();
  }
}

TEST_F(CppFeature, vectorReserve) {
  std::vector<int> nums(10);
  std::iota(nums.begin(), nums.end(), 1);
  std::copy(nums.begin(), nums.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  nums.reserve(5);
  EXPECT_EQ(nums.size(), 10);
  EXPECT_EQ(nums.capacity(), 10);
  std::copy(nums.begin(), nums.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  nums.resize(5);
  EXPECT_EQ(nums.size(), 5);
  EXPECT_EQ(nums.capacity(), 10);
  std::copy(nums.begin(), nums.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  nums.reserve(5);
  EXPECT_EQ(nums.size(), 5);
  EXPECT_EQ(nums.capacity(), 10);
  std::copy(nums.begin(), nums.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;

  nums.shrink_to_fit();
  EXPECT_EQ(nums.size(), 5);
  EXPECT_EQ(nums.capacity(), 5);
  std::copy(nums.begin(), nums.end(),
            std::ostream_iterator<int>(std::cout, " "));
  std::cout << std::endl;
}

TEST_F(CppFeature, STL_Copy) {
  std::string sentence = " hello ma shoujiang   nice to meet you!   ";
  std::vector<std::string> tokens;
  std::istringstream iss(sentence);
  std::copy(std::istream_iterator<std::string>(iss),
            std::istream_iterator<std::string>(), std::back_inserter(tokens));

  std::copy(tokens.begin(), tokens.end(),
            std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << "There is a space before this sentence." << std::endl;

  std::istringstream anotherIss(sentence);
  std::vector<std::string> anotherTokens{std::istream_iterator<std::string>{anotherIss}, std::istream_iterator<std::string>{}};
  std::copy(anotherTokens.begin(), anotherTokens.end(),
            std::ostream_iterator<std::string>(std::cout, " "));
  std::cout << "There is a space before this sentence." << std::endl;

  std::cout << std::endl;
}
