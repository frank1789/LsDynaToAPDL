set(LIBRARY_TARGET_NAME "common")

# List of (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/filemanager.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/element_shell_four_node.cc
  ${CMAKE_CURRENT_SOURCE_DIR}/src/elementproperty.cc
)

# List of (header) library files.
set(${LIBRARY_TARGET_NAME}_HDR
  ${CMAKE_CURRENT_SOURCE_DIR}/include/finite_element_types.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/elementproperty.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/node_type_impl.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/filemanager.h
  ${CMAKE_CURRENT_SOURCE_DIR}/include/element_shell_four_node.h
)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})
target_link_libraries(${LIBRARY_TARGET_NAME} 
  PRIVATE 
  fmt::fmt 
  ${PROJECT_NAME}::logtools
)

if(CMAKE_BUILD_TYPE MATCHES "Debug" AND COVERAGE)
  target_link_libraries(${LIBRARY_TARGET_NAME} coverage)
endif()

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${CMAKE_PROJECT_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}"
)

target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>"
)

install(TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT  ${PROJECT_NAME}
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
  RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev
  FILE_SET HEADERS
)

message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
