set(TEST_TARGET_NAME LsDynaToApdlTests)

file(GLOB_RECURSE ${TEST_TARGET_NAME}_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/**.test.cc)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRCS})

target_link_libraries(${TEST_TARGET_NAME}
  PRIVATE
  GTest::gtest
  GTest::gtest_main
  GTest::gmock
  GTest::gmock_main
  fmt::fmt
  ${PROJECT_NAME}::common
  ${PROJECT_NAME}::lsdyna
)

target_compile_definitions(${TEST_TARGET_NAME} PRIVATE TEST_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

gtest_add_tests(TARGET ${TEST_TARGET_NAME})
gtest_discover_tests(${TEST_TARGET_NAME})

add_custom_command(
  TARGET ${TEST_TARGET_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/example.k
    $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>
)
