file(GLOB_RECURSE TEST_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.cc")

foreach(TEST_FILE ${TEST_FILES})
  get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)

  add_executable(${TEST_NAME} ${TEST_FILE})

  target_link_libraries(${TEST_NAME} gtest gtest_main InterviewQuestions)
  
  include(GoogleTest)

  gtest_discover_tests(${TEST_NAME})
endforeach()
