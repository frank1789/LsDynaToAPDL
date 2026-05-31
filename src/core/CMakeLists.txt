set(EXECUTABLE_TARGET_NAME "core")

add_executable(${EXECUTABLE_TARGET_NAME})
target_sources(
  ${EXECUTABLE_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         ${CMAKE_CURRENT_SOURCE_DIR}/include
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/core/parser.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/core/core.h)

target_link_libraries(
  ${EXECUTABLE_TARGET_NAME}
  PRIVATE Threads::Threads
          fmt::fmt
          spdlog::spdlog
          gflags::gflags
          ${PROJECT_NAME}::lsdyna
          ${PROJECT_NAME}::common
          ${PROJECT_NAME}::logtools
          ${PROJECT_NAME}::apdl)

install(
  TARGETS ${EXECUTABLE_TARGET_NAME}
  BUNDLE DESTINATION . COMPONENT Runtime
  RUNTIME DESTINATION bin
          COMPONENT Runtime
          FILE_SET HEADERS
          DESTINATION include/${PROJECT_NAME}
          COMPONENT Development)
