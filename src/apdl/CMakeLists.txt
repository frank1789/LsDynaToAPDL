set(LIBRARY_TARGET_NAME apdl)

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/deck_writer.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/apdl_entity.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/deck_writer.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/keypoint.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/material.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/model.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/node.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/section.hh
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/shell181.hh)

# fmt is PUBLIC: the CRTP entities format through it in their headers.
target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC fmt::fmt
                      PRIVATE spdlog::spdlog)

lsdynatoapdl_configure_target(${LIBRARY_TARGET_NAME})

set_target_properties(
  ${LIBRARY_TARGET_NAME}
  PROPERTIES VERSION ${CMAKE_PROJECT_VERSION} OUTPUT_NAME lsdynatoapdl_apdl)

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
