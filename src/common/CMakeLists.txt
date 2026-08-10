set(LIBRARY_TARGET_NAME common)

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/file_manager.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/common/file_manager.hh)

# fmt is PUBLIC because it appears in file_manager.hh; spdlog is used only in
# the implementation, so it stays out of the interface consumers inherit.
target_link_libraries(
  ${LIBRARY_TARGET_NAME}
  PUBLIC fmt::fmt
  PRIVATE spdlog::spdlog ${PROJECT_NAME}::logtools)

lsdynatoapdl_configure_target(${LIBRARY_TARGET_NAME})

set_target_properties(
  ${LIBRARY_TARGET_NAME}
  PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} OUTPUT_NAME lsdynatoapdl_common)

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
