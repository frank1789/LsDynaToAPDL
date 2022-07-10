add_executable(log test_logger.cc)
add_executable(femtypes test_femtypes.cc)
add_executable(converter test_converter.cc)
add_executable(managerfile test_managerfile.cc)

target_link_libraries(
  log Qt5::Core gtest_main ${PROJECT_NAME}::logger ${PROJECT_NAME}::lsdyna
)

target_link_libraries(
  femtypes Qt5::Core gtest_main ${PROJECT_NAME}::logger ${PROJECT_NAME}::lsdyna
)

target_link_libraries(
  converter Qt5::Core gtest_main ${PROJECT_NAME}::logger ${PROJECT_NAME}::lsdyna
)

target_link_libraries(
  managerfile Qt5::Core gtest_main ${PROJECT_NAME}::logger ${PROJECT_NAME}::lsdyna
)

if(COVERAGE)
  target_link_libraries(log coverage)
  target_link_libraries(femtypes coverage)
  target_link_libraries(converter coverage)
  target_link_libraries(managerfile coverage)
endif(COVERAGE)

add_test(NAME test-log
         COMMAND log
         WORKING_DIRECTORY $<TARGET_FILE_DIR:log>
)

add_test(NAME test-femtypes
         COMMAND femtypes
         WORKING_DIRECTORY $<TARGET_FILE_DIR:femtypes>
)

add_test(NAME test-converter
         COMMAND converter
         WORKING_DIRECTORY $<TARGET_FILE_DIR:converter>
)

add_test(NAME test-managerfile
         COMMAND managerfile
         WORKING_DIRECTORY $<TARGET_FILE_DIR:managerfile>
)

add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --verbose)


add_custom_command(
  TARGET managerfile POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_CURRENT_SOURCE_DIR}/example.k
    $<TARGET_FILE_DIR:managerfile>
)
