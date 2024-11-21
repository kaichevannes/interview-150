# interview-150

Solutions to the leetcode top interview 150: https://leetcode.com/studyplan/top-interview-150/

Initialise using:
cmake -G Ninja ..

Rebuild and run tests using:
ninja && ctest --output_on_failure -R TestSuiteName

To run only unit tests add:
-E Property

To run only the property tests:
-R TestSuiteNameProperty

To avoid having to add the --output-on-failure flag when manually testing, add
this environment variable:
export CTEST_OUTPUT_ON_FAILURE=1
