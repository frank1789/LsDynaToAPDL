cmake_minimum_required(VERSION 3.16)
project(LsDynaToApdl LANGUAGES CXX)
set(NAME_PROJECT "LsDynaToApdl")

# -----------------------------------------------------------------------------
# load cmake module
# -----------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")
include( GitVersion )
include(cmake/googletest.cmake)

# -----------------------------------------------------------------------------
# Google test
# -----------------------------------------------------------------------------
fetch_googletest(${PROJECT_SOURCE_DIR}/cmake ${PROJECT_BINARY_DIR}/googletest)
enable_testing()
if(NOT TEST_NO_DEBUG_OUTPUT)
  set(TEST_NO_DEBUG_OUTPUT "ON")
  message(STATUS "Compile without Qt debug output")
  add_definitions(-D QT_NO_DEBUG_OUTPUT)
endif()

# -----------------------------------------------------------------------------
# check build type
# -----------------------------------------------------------------------------
if(NOT CMAKE_BUILD_TYPE)
  message(STATUS "Compile in debug mode")
  set(CMAKE_BUILD_TYPE Debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
  message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
  set(CMAKE_BUILD_TYPE Debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
  message(STATUS "CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")
  set(CMAKE_BUILD_TYPE Release)
endif()

# -----------------------------------------------------------------------------
# C/C++ standard extensions
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# -----------------------------------------------------------------------------
# Code coverage
# -----------------------------------------------------------------------------
if(COVERAGE)
  set(COVERAGE_CHECK "ON")
  add_library(coverage INTERFACE)
  if(${CMAKE_BUILD_TYPE} STREQUAL "Debug" AND CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(
      coverage
      INTERFACE -O0 # no optimization
                -g # generate debug info
                -ftest-coverage -fprofile-arcs # sets all required flags
    )
    if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.13)
      target_link_options(coverage INTERFACE --coverage)
    else()
      target_link_libraries(coverage INTERFACE --coverage)
    endif()
  endif()
endif()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed
set(CMAKE_AUTOMOC ON)
# Create code from a list of Qt designer ui files
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# -----------------------------------------------------------------------------
# Library and Dependecies
# -----------------------------------------------------------------------------
# Find the Qt library
if(APPLE AND EXISTS /usr/local/opt/qt5)
  list(APPEND CMAKE_PREFIX_PATH "/usr/local/opt/qt5")
endif()
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

if(${Qt5_FOUND})
  message(STATUS "Found Qt 5")
else(${Qt5_FOUND})
  message(FATAL_ERROR "Could not locate Qt.")
endif(${Qt5_FOUND})

# Find PThread Library
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

set(CMAKE_C_FLAGS_DEBUG "-march=native -pthread")
set(CMAKE_CXX_FLAGS_DEBUG "-march=native -pthread -Wall -Wextra -Wunused -Wshadow -Wredundant-decls -Wcast-align -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wredundant-decls -Wmissing-format-attribute -Wformat-nonliteral -fstack-protector-strong")
set(CMAKE_C_FLAGS_RELEASE "-march=native -pthread")
set(CMAKE_CXX_FLAGS_RELEASE "-DNDEBUG -march=native -pthread -Os -fpic -Wall -Wextra -Wunused -Wshadow -Wredundant-decls -Wcast-align -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wredundant-decls -Wmissing-format-attribute -Wformat-nonliteral -fstack-protector-strong")

# -----------------------------------------------------------------------------
# Build Documentation
# -----------------------------------------------------------------------------
option(BUILD_DOC "Build documentation" OFF)
# check if Doxygen is installed
if(BUILD_DOC)
  find_package(Doxygen)
  if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)
    # request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message(STATUS "Doxygen build started")
    # note the option ALL which allows to build the docs together with the
    # application
    add_custom_target(
      doc_doxygen ALL
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen"
      VERBATIM)
  else(DOXYGEN_FOUND)
    message(STATUS "Doxygen need to be installed to generate the doxygen documentation")
  endif(DOXYGEN_FOUND)
endif()

# -----------------------------------------------------------------------------
# Local library and dependecies
# -----------------------------------------------------------------------------
add_subdirectory(src)
add_subdirectory(test)

# ----------------------------------------------------------------------------
# display status message for important variables
# ----------------------------------------------------------------------------
message(STATUS)
message(STATUS "-------------------------------------------------------------------------------")
message(STATUS "General configuration for ${PROJECT_NAME} ${VERSION_SHORT}")
message(STATUS "-------------------------------------------------------------------------------")
message(STATUS)
message(STATUS "Compiler: ${CMAKE_COMPILER}\t${CMAKE_CXX_COMPILER}")
message(STATUS "C++ flags (Debug): ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG}")
message(STATUS "C++ flags (Release): ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
message(STATUS "Coverage:\t\t${COVERAGE}")
message(STATUS "Documentation:\t${BUILD_DOC}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS)
message(STATUS "Change a value with: cmake -D<Variable>=<Value> ..")
message(STATUS)
