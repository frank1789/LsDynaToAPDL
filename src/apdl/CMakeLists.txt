set(LIBRARY_TARGET_NAME "apdl")

set(${LIBRARY_TARGET_NAME}_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/src/writeapdl.cc
)

set(${LIBRARY_TARGET_NAME}_HDR
  ${CMAKE_CURRENT_SOURCE_DIR}/include/apdl/writeapdl.h
)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})
add_library(${PROJECT_NAME}::${LIBRARY_TARGET_NAME} ALIAS ${LIBRARY_TARGET_NAME})

target_link_libraries(${LIBRARY_TARGET_NAME}
  PUBLIC
  ${PROJECT_NAME}::common
  spdlog::spdlog
  fmt::fmt
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
)

message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
