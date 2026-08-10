set(LIBRARY_TARGET_NAME lsdyna)

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/converter.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_element.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/parser_node.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/converter.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/fields.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/keywords.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/parser.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/parser_element.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/parser_node.hh)

# apdl and fmt are part of the interface (converter.hh exposes apdl::Model,
# keywords.hh formats through fmt); spdlog is an implementation detail.
target_link_libraries(
  ${LIBRARY_TARGET_NAME}
  PUBLIC fmt::fmt ${PROJECT_NAME}::apdl
  PRIVATE spdlog::spdlog)

# fields.hh selects its double parser from this; consumers of the installed
# headers need the same verdict, so it travels with the interface.
target_compile_definitions(
  ${LIBRARY_TARGET_NAME}
  PUBLIC LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS=${LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS})

lsdynatoapdl_configure_target(${LIBRARY_TARGET_NAME})

set_target_properties(
  ${LIBRARY_TARGET_NAME}
  PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} OUTPUT_NAME lsdynatoapdl_lsdyna)

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
