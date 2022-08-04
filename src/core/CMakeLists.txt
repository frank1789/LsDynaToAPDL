set(EXECUTABLE_TARGET_NAME "core")

add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME}
  PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
  PRIVATE FILE_SET HEADERS 
  BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include
  FILES 
  ${CMAKE_CURRENT_SOURCE_DIR}/include/core/parser.h
)

target_link_libraries(${PROJECT_NAME} PRIVATE
  Threads::Threads
  fmt::fmt
  spdlog::spdlog
  gflags::gflags
  ${PROJECT_NAME}::lsdyna
  ${PROJECT_NAME}::common
  ${PROJECT_NAME}::logtools
  ${PROJECT_NAME}::apdl
)

if(CMAKE_BUILD_TYPE MATCHES "Debug" AND COVERAGE)
  target_link_libraries(coverage)
endif()

install(TARGETS ${NAME_PROJECT}
    BUNDLE DESTINATION . COMPONENT Runtime
    RUNTIME DESTINATION bin COMPONENT Runtime
)

