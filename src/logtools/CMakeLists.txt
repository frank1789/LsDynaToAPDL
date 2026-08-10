set(LIBRARY_TARGET_NAME logtools)

# Header-only: an INTERFACE library has nothing to compile, so warnings,
# coverage and sanitizers do not apply to it. The previous file called
# target_link_libraries(coverage) with a single argument, which is a hard
# CMake error whenever COVERAGE was actually switched on.
add_library(${LIBRARY_TARGET_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  INTERFACE FILE_SET
            HEADERS
            BASE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/include"
            FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/include/customassert.h
            ${CMAKE_CURRENT_SOURCE_DIR}/include/logtools/logger_tools.h)

target_link_libraries(${LIBRARY_TARGET_NAME} INTERFACE spdlog::spdlog)

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
