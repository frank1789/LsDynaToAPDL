cmake_minimum_required(VERSION 3.16)
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
target_link_libraries(${LIBRARY_TARGET_NAME} Qt5::Core logger)
target_compile_options(${LIBRARY_TARGET_NAME} PRIVATE -c -fPIC -fstack-protector-strong)
if(CMAKE_BUILD_TYPE MATCHES "Debug")
  if(COVERAGE)
    target_link_libraries(${LIBRARY_TARGET_NAME} coverage)
  endif()
endif()

# Set two minimum target properties for the library.
# See https://cmake.org/cmake/help/latest/command/set_target_properties.html
# Properties are: 1) Library version number 2) list of corresponding public headers
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES VERSION       ${${PROJECT_NAME}_VERSION}
                                                        PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}"
                                                        BUNDLE true
)


# Specify include directories for both compilation and installation process.
# The $<INSTALL_PREFIX> generator expression is useful to ensure to create
# relocatable configuration files, see https://cmake.org/cmake/help/latest/manual/cmake-packages.7.html#creating-relocatable-packages
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
                                                         "$<INSTALL_INTERFACE:$<INSTALL_PREFIX>/${CMAKE_INSTALL_INCLUDEDIR}>")

# Specify installation targets, typology and destination folders.
if(APPLE)
  install(TARGETS ${LIBRARY_TARGET_NAME}
          BUNDLE # DLLs are treated as RUNTIME, .so's are treated as LIBRARY, and .a and .lib as ARCHIVE, MacOSX bundles as BUNDLE
          LIBRARY       DESTINATION "${INSTALL_LIBRARY_DIR}"                                  COMPONENT shlib
          ARCHIVE       DESTINATION "${INSTALL_LIBRARY_DIR}"                                  COMPONENT lib
          RUNTIME       DESTINATION "${INSTALL_BINDIR}"                                       COMPONENT bin
          PUBLIC_HEADER DESTINATION "${prefix}/${INSTALL_INCLUDE_DIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev
        )
elseif(UNIX AND NOT APPLE) #Linux
      install(TARGETS ${LIBRARY_TARGET_NAME}
        EXPORT  ${PROJECT_NAME}
        LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
        ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
        RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
        PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${LIBRARY_TARGET_NAME}" COMPONENT dev
      )
endif()


message(STATUS "Created target ${LIBRARY_TARGET_NAME} for export ${PROJECT_NAME}.")
