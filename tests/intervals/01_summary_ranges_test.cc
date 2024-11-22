#include "../../src/intervals/01_summary_ranges.h"
#include <gtest/gtest.h>
#include <rapidcheck.h>
#include <rapidcheck/gtest.h>
#include <string>

class SummaryRangesTest : public testing::Test {
protected:
  SummaryRanges summaryRanges;
  std::vector<int> nums;
};

TEST_F(SummaryRangesTest, EmptyNums) {
  nums = {};
  EXPECT_EQ(std::vector<std::string>({}), summaryRanges.summaryRanges(nums));
}

TEST_F(SummaryRangesTest, ConsecutiveNumbers) {
  nums = {0, 1, 2};
  EXPECT_EQ(std::vector<std::string>({"0->2"}),
            summaryRanges.summaryRanges(nums));
}

TEST_F(SummaryRangesTest, SingleNumber) {
  nums = {0};
  EXPECT_EQ(std::vector<std::string>({"0"}), summaryRanges.summaryRanges(nums));
}

TEST_F(SummaryRangesTest, SingleNegativeNumber) {
  nums = {-1};
  EXPECT_EQ(std::vector<std::string>({"-1"}), summaryRanges.summaryRanges(nums));
}

class SummaryRangesTestProperty : public SummaryRangesTest {
protected:
  rc::Gen<std::vector<int>> genNums() {
    return rc::gen::map(genUniqueIntVector(), [](std::vector<int> nums) {
      std::sort(nums.begin(), nums.end());
      return nums;
    });
  }

  std::pair<int, int> parseRange(std::string range) {
    assert(range.size() > 1);

    RC_LOG() << "parsing " << range << std::endl;
    auto pos = range.find("->");
    int start = std::stoi(range.substr(0, pos));
    RC_LOG() << "start = " << start << std::endl;
    int end = std::stoi(range.substr(pos + 2));

    return {start, end};
  }

  bool isRange(std::string range) {
    return range.find("->") != std::string::npos;
  }

private:
  rc::Gen<std::vector<int>> genUniqueIntVector() {
    return rc::gen::unique<std::vector<int>>(rc::gen::arbitrary<int>());
  }
};

RC_GTEST_FIXTURE_PROP(SummaryRangesTestProperty, SolutionSizeIsLessThanNums,
                      ()) {
  nums = *genNums();
  RC_ASSERT(summaryRanges.summaryRanges(nums).size() <= nums.size());
}

RC_GTEST_FIXTURE_PROP(SummaryRangesTestProperty, RangesAreValid, ()) {
  nums = *genNums();
  std::vector<std::string> ranges = summaryRanges.summaryRanges(nums);

  for (std::string range : ranges) {
    if (!isRange(range))
      continue;

    auto [start, end] = parseRange(range);

    RC_ASSERT(start < end);
  }
}

RC_GTEST_FIXTURE_PROP(SummaryRangesTestProperty, RangesBelongToNums, ()) {
  nums = *genNums();
  std::vector<std::string> ranges = summaryRanges.summaryRanges(nums);

  for (std::string range : ranges) {
    if (!isRange(range)) {
      RC_ASSERT(std::find(nums.begin(), nums.end(), std::stoi(range)) != nums.end());
      continue;
    }

    auto [start, end] = parseRange(range);

    for (int i = start; i < end; i ++) {
      RC_ASSERT(std::find(nums.begin(), nums.end(), i) != nums.end());
    }
  }
}
