set(LIBRARY_TARGET_NAME lsdyna)

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/converter.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/keywords.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/node.cc
          ${CMAKE_CURRENT_SOURCE_DIR}/src/lsdyna.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/converter.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/keywords.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/node.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/lsdyna.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/parser_element.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/parser_node.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdyna/parser.hh)

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC fmt::fmt spdlog::spdlog
                                                    ${PROJECT_NAME}::common)

set_target_properties(${LIBRARY_TARGET_NAME}
                      PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} BUNDLE true)

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
