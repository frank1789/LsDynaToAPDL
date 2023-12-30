include(GoogleTest)
set(PREFIX_TESTS "Test")


file(GLOB_RECURSE ${PREFIX_TESTS}_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/**.test.cc)
message("${${PREFIX_TESTS}_SRCS}")
#add_executable(log test_logger.cc)
#add_executable(femtypes test_femtypes.cc)
#add_executable(converter test_converter.cc)


add_executable(${PREFIX_TESTS}FileManager ${${PREFIX_TESTS}_SRCS})

target_link_libraries(${PREFIX_TESTS}FileManager 
  PRIVATE 
  GTest::gtest 
  GTest::gtest_main 
  GTest::gmock 
  GTest::gmock_main
  fmt::fmt
  ${PROJECT_NAME}::common
  ${PROJECT_NAME}::lsdyna
)

gtest_add_tests(TARGET ${PREFIX_TESTS}FileManager)
gtest_discover_tests(${PREFIX_TESTS}FileManager)
#target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)

# if(COVERAGE)
#   target_link_libraries(log coverage)
#   target_link_libraries(femtypes coverage)
#   target_link_libraries(converter coverage)
#   target_link_libraries(managerfile coverage)
# endif(COVERAGE)

# add_test(NAME test-log
#          COMMAND log
#          WORKING_DIRECTORY $<TARGET_FILE_DIR:log>
# )

# add_test(NAME test-femtypes
#          COMMAND femtypes
#          WORKING_DIRECTORY $<TARGET_FILE_DIR:femtypes>
# )

# add_test(NAME test-converter
#          COMMAND converter
#          WORKING_DIRECTORY $<TARGET_FILE_DIR:converter>
# )

# add_test(NAME test-managerfile
#          COMMAND managerfile
#          WORKING_DIRECTORY $<TARGET_FILE_DIR:managerfile>
# )

# add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)


add_custom_command(
  TARGET ${PREFIX_TESTS}FileManager POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/example.k
    $<TARGET_FILE_DIR:${PREFIX_TESTS}FileManager>
)

#gtest_discover_tests(tests WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
