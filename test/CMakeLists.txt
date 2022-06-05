add_executable(log test_logger.cc)
add_executable(femtypes test_femtypes.cc)
add_executable(converter test_converter.cc)
add_executable(managerfile test_managerfile.cc)

target_link_libraries(
  log Qt5::Core gtest_main logger lsdyna
)

target_link_libraries(
  femtypes Qt5::Core gtest_main logger lsdyna
)

target_link_libraries(
  converter Qt5::Core gtest_main logger lsdyna
)

target_link_libraries(
  managerfile Qt5::Core gtest_main logger lsdyna
)

if(COVERAGE)
  target_link_libraries(log coverage)
  target_link_libraries(femtypes coverage)
  target_link_libraries(converter coverage)
  target_link_libraries(managerfile coverage)
endif(COVERAGE)

add_test(NAME test-log COMMAND ${CMAKE_BINARY_DIR}/test/log)
add_test(NAME test-femtypes COMMAND ${CMAKE_BINARY_DIR}/test/femtypes)
add_test(NAME test-converter COMMAND ${CMAKE_BINARY_DIR}/test/converter)
add_test(NAME test-managerfile COMMAND ${CMAKE_BINARY_DIR}/test/managerfile)
