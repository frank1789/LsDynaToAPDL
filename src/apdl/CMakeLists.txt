set(LIBRARY_TARGET_NAME apdl)

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS
            ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/writeapdl.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/writeapdl.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/keypoint.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/node.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/writeapdl.h)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE common
                                                     spdlog::spdlog fmt::fmt)

set_target_properties(${LIBRARY_TARGET_NAME}
                      PROPERTIES VERSION ${CMAKE_PROJECT_VERSION})

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
