#include "sliding_window/01_minimum_size_subarray_sum.h"
#include <algorithm>
#include <climits>
#include <gtest/gtest.h>
#include <numeric>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>

class MinimumSizeSubArraySumTest : public testing::Test {
protected:
  MinimumSizeSubArraySum minimumSizeSubArraySum;
  int target;
  std::vector<int> nums;
};

TEST_F(MinimumSizeSubArraySumTest, SubArrayMatchesTarget) {
  target = 2;
  nums = {2};
  EXPECT_EQ(1, minimumSizeSubArraySum.minSubArrayLen(target, nums));
}

TEST_F(MinimumSizeSubArraySumTest, NoSuchSubArray) {
  target = 100;
  nums = {1, 2, 3, 4, 5};
  EXPECT_EQ(0, minimumSizeSubArraySum.minSubArrayLen(target, nums));
}

TEST_F(MinimumSizeSubArraySumTest, SpreadOutSubArray) {
  target = 7;
  nums = {1, 1, 3, 4, 1};
  EXPECT_EQ(2, minimumSizeSubArraySum.minSubArrayLen(target, nums));
}

TEST_F(MinimumSizeSubArraySumTest, DuplicateInSubArray) {
  target = 4;
  nums = {1, 4, 4};
  EXPECT_EQ(1, minimumSizeSubArraySum.minSubArrayLen(target, nums));
}

TEST_F(MinimumSizeSubArraySumTest, LongSubArray) {
  target = 10;
  nums = {1, 1, 1, 1, 1, 1, 1, 1, 1, 1};
  EXPECT_EQ(10, minimumSizeSubArraySum.minSubArrayLen(target, nums));
}

TEST_F(MinimumSizeSubArraySumTest, ContiguousSubArray) {
  target = 7;
  nums = {3, 1, 4};
  EXPECT_EQ(3, minimumSizeSubArraySum.minSubArrayLen(target, nums));
}

class MinimumSizeSubArraySumTestProperty : public MinimumSizeSubArraySumTest {
protected:
  int defaultTarget() { return *rc::gen::inRange(1,100000); }

  std::vector<int> defaultNums() {
    return *rc::gen::suchThat(
        rc::gen::container<std::vector<int>>(rc::gen::inRange(1,10000)),
        [](std::vector<int> vec) { return vec.size() > 0; });
  }
};

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      ReturnsZeroWhenTargetIsMaximumInteger, ()) {
  target = INT_MAX;
  nums = defaultNums();
  RC_ASSERT(minimumSizeSubArraySum.minSubArrayLen(target, nums) == 0);
}

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      SolutionExistsWhenArraySizeExceedsTarget, ()) {
  target = *rc::gen::inRange(1, 50);
  std::vector<int> nums =
      *rc::gen::mapcat(rc::gen::inRange<int>(target, 100), [](int size) {
        return rc::gen::container<std::vector<int>>(size,
                                                    rc::gen::positive<int>());
      });
  RC_ASSERT(minimumSizeSubArraySum.minSubArrayLen(target, nums) != 0);
}

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      PreservesSolutionWhenArrayElementsAreIncremented, ()) {
  target = defaultTarget();
  nums = defaultNums();
  RC_PRE(minimumSizeSubArraySum.minSubArrayLen(target, nums) != 0);

  std::transform(nums.begin(), nums.end(), nums.begin(),
                 [](int val) { return val + 1; });
  RC_ASSERT(minimumSizeSubArraySum.minSubArrayLen(target, nums) != 0);
}

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      ResultEqualsTargetWhenAllElementsAreOne, ()) {
  target = *rc::gen::inRange(1, 100);
  nums = *rc::gen::container<std::vector<int>>(rc::gen::just<int>(1));
  RC_PRE(nums.size() >= target);

  RC_ASSERT(minimumSizeSubArraySum.minSubArrayLen(target, nums) == target);
}

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      EnsuresArraySumExceedsTargetForValidSolution, ()) {
  target = defaultTarget();
  nums = defaultNums();
  RC_PRE(minimumSizeSubArraySum.minSubArrayLen(target, nums) != 0);

  long long sum = std::accumulate(nums.begin(), nums.end(), 0LL);
  RC_ASSERT(sum >= target);
}

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      ReturnsOneWhenTargetIsOne, ()) {
  target = 1;
  nums = defaultNums();
  RC_ASSERT(minimumSizeSubArraySum.minSubArrayLen(target, nums) == 1);
}

RC_GTEST_FIXTURE_PROP(MinimumSizeSubArraySumTestProperty,
                      ReturnsOneWhenElementExceedsTarget, ()) {
  target = defaultTarget();
  nums = defaultNums();
  nums.push_back(target + 1);
  RC_ASSERT(minimumSizeSubArraySum.minSubArrayLen(target, nums) == 1);
}
