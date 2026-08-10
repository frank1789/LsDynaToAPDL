set(EXECUTABLE_TARGET_NAME core)

add_executable(${EXECUTABLE_TARGET_NAME})

# The target is called "core" for consistency with the other modules, but the
# installed program is the tool itself, and that is the name documented in the
# README and used on the command line.
set_target_properties(${EXECUTABLE_TARGET_NAME} PROPERTIES OUTPUT_NAME
                                                           lsdynatoapdl)

target_sources(
  ${EXECUTABLE_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/parser.cc
  PRIVATE FILE_SET
          HEADERS
          BASE_DIRS
          ${CMAKE_CURRENT_SOURCE_DIR}/include
          FILES
          ${CMAKE_CURRENT_SOURCE_DIR}/include/core/parser.hh)

target_link_libraries(
  ${EXECUTABLE_TARGET_NAME}
  PRIVATE Threads::Threads
          fmt::fmt
          spdlog::spdlog
          gflags::gflags
          ${PROJECT_NAME}::apdl
          ${PROJECT_NAME}::common
          ${PROJECT_NAME}::logtools
          ${PROJECT_NAME}::lsdyna)

lsdynatoapdl_configure_target(${EXECUTABLE_TARGET_NAME})

install(
  TARGETS ${EXECUTABLE_TARGET_NAME}
  BUNDLE DESTINATION . COMPONENT Runtime
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT Runtime)
