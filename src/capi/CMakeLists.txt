set(LIBRARY_TARGET_NAME lsdynatoapdl_c)

add_library(${LIBRARY_TARGET_NAME})
add_library(${PROJECT_NAME}::capi ALIAS ${LIBRARY_TARGET_NAME})

target_sources(
  ${LIBRARY_TARGET_NAME}
  PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src/c_api.cc
  PUBLIC FILE_SET
         HEADERS
         BASE_DIRS
         "${CMAKE_CURRENT_SOURCE_DIR}/include"
         FILES
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdynatoapdl/export.h
         ${CMAKE_CURRENT_SOURCE_DIR}/include/lsdynatoapdl/lsdynatoapdl.h)

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE ${PROJECT_NAME}::apdl
                                                     ${PROJECT_NAME}::lsdyna)

# The export macros need to know which of the three build situations applies;
# see include/lsdynatoapdl/export.h.
target_compile_definitions(
  ${LIBRARY_TARGET_NAME}
  PRIVATE LSDYNATOAPDL_EXPORTS LSDYNATOAPDL_VERSION="${CMAKE_PROJECT_VERSION}")
if(NOT BUILD_SHARED_LIBS)
  target_compile_definitions(${LIBRARY_TARGET_NAME}
                             PUBLIC LSDYNATOAPDL_STATIC_DEFINE)
endif()

# Hide everything that is not explicitly exported, so the shared library
# publishes only the C API and not the C++ symbols behind it.
set_target_properties(
  ${LIBRARY_TARGET_NAME}
  PROPERTIES VERSION ${CMAKE_PROJECT_VERSION}
             C_VISIBILITY_PRESET hidden
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN ON)

lsdynatoapdl_configure_target(${LIBRARY_TARGET_NAME})

install(
  TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT ${PROJECT_NAME}
  LIBRARY DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT shlib
  ARCHIVE DESTINATION "${CMAKE_INSTALL_LIBDIR}" COMPONENT lib
  RUNTIME DESTINATION "${CMAKE_INSTALL_BINDIR}" COMPONENT bin
  FILE_SET HEADERS DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}" COMPONENT dev)

message(
  STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
