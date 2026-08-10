set(TEST_TARGET_NAME LsDynaToApdlTests)

# Tests are named <subject>_test.cc; listing them explicitly keeps CMake from
# silently producing an empty target when a file is renamed.
set(${TEST_TARGET_NAME}_SRCS
    ${CMAKE_CURRENT_SOURCE_DIR}/main_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/apdl_entity_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/c_api_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/converter_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/element_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/filemanager_test.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/keywords_test.cc)

add_executable(${TEST_TARGET_NAME} ${${TEST_TARGET_NAME}_SRCS})

target_link_libraries(
  ${TEST_TARGET_NAME}
  PRIVATE GTest::gtest
          GTest::gtest_main
          GTest::gmock
          fmt::fmt
          spdlog::spdlog
          ${PROJECT_NAME}::apdl
          ${PROJECT_NAME}::capi
          ${PROJECT_NAME}::common
          ${PROJECT_NAME}::lsdyna)

lsdynatoapdl_configure_target(${TEST_TARGET_NAME})

target_compile_definitions(
  ${TEST_TARGET_NAME} PRIVATE TEST_FILE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

gtest_discover_tests(${TEST_TARGET_NAME})

add_custom_command(
  TARGET ${TEST_TARGET_NAME}
  POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_CURRENT_SOURCE_DIR}/example.k
          $<TARGET_FILE_DIR:${TEST_TARGET_NAME}>)
