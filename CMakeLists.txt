cmake_minimum_required(VERSION 3.18)
project(LsDynaToApdl LANGUAGES CXX C VERSION 0.0.0)
set(NAME_PROJECT "LsDynaToApdl")
set(CMAKE_MODULE_PATH ${LsDynaToApdl_SOURCE_DIR}/cmake)

include(GNUInstallDirs)

# Control where libraries and executables are placed during the build.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

if(APPLE)
  set(MACOSX_BUNDLE_NAME "LsDynaToAPDL")
  set(prefix "${MACOSX_BUNDLE_NAME}.app/Contents")
  set(INSTALL_INCLUDE_DIR "${prefix}/${INSTALL_INCLUDE_DIR}")
  set(INSTALL_RUNTIME_DIR "${prefix}/MacOS")
  set(INSTALL_LIBRARY_DIR "${prefix}/${INSTALL_LIBRARY_DIR}")
  set(INSTALL_ARCHIVE_DIR "${prefix}/${INSTALL_ARCHIVE_DIR}")
  set(INSTALL_DATA_DIR    "${prefix}/${INSTALL_DATA_DIR}")
  set(INSTALL_DOC_DIR     "${prefix}/${INSTALL_DOC_DIR}")
  set(INSTALL_CMAKE_DIR   "${prefix}/Resources")
endif()


# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS to TRUE.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Under MSVC, we set CMAKE_DEBUG_POSTFIX to "d" to add a trailing "d" to library
# built in debug mode. In this Windows user can compile, build and install the
# library in both Release and Debug configuration avoiding naming clashes in the
# installation directories.
if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

# Build position independent code.
# Position Independent Code (PIC) is commonly used for shared libraries so that
# the same shared library code can be loaded in each program address space in a
# location where it will not overlap with any other uses of such memory.
# In particular, this option avoids problems occurring when a process wants to
# load more than one shared library at the same virtual address.
# Since shared libraries cannot predict where other shared libraries could be
# loaded, this is an unavoidable problem with the traditional shared library
# concept.
# Generating position-independent code is often the default behavior for most
# modern compilers.
set(CMAKE_POSITION_INDEPENDENT_CODE ON)


# -----------------------------------------------------------------------------
# load cmake module
# -----------------------------------------------------------------------------
include( GitVersion )
include(cmake/googletest.cmake)
set(${PROJECT_VERSION_MAJOR} "${VERSION_MAJOR}")
set(${PROJECT_VERSION_MINOR} "${VERSION_MINOR}")
set(${PROJECT_VERSION_PATCH} "${VERSION_PATCH}")


# -----------------------------------------------------------------------------
# Scripting
# -----------------------------------------------------------------------------
# include(cmake/sol2.cmake)
# fetch_sol2(${PROJECT_SOURCE_DIR}/cmake ${PROJECT_BINARY_DIR}/sol2)
# find_package(Lua 5 REQUIRED)
# include_directories(${LUA_INCLUDE_DIR})


# Build test related commands
option(BUILD_TESTING "Create tests using CMake" OFF)
if(BUILD_TESTING)
  # -----------------------------------------------------------------------------
  # Google test
  # -----------------------------------------------------------------------------
  fetch_googletest(${PROJECT_SOURCE_DIR}/cmake ${PROJECT_BINARY_DIR}/googletest)
  
  enable_testing()
  add_definitions(-D QT_NO_DEBUG_OUTPUT)

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
endif()


# -----------------------------------------------------------------------------
# check build type
# -----------------------------------------------------------------------------
# Enable RPATH support for installed binaries and libraries
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_FULL_BINDIR}"
                          LIB_DIRS "${CMAKE_INSTALL_FULL_LIBDIR}"
                          INSTALL_NAME_DIR "${CMAKE_INSTALL_FULL_LIBDIR}"
                          USE_LINK_PATH)

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Release.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
        add_definitions(-D QT_NO_DEBUG_OUTPUT)
    endif()
endif()


# -----------------------------------------------------------------------------
# C/C++ standard extensions
# -----------------------------------------------------------------------------
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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

find_package(Qt5 COMPONENTS Core Concurrent REQUIRED)
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


set(CMAKE_CXX_FLAGS_DEBUG   "${CMAKE_CXX_FLAGS_DEBUG} -O0 -march=native -Wall -Wextra -Wunused -Wshadow -Wredundant-decls -Wcast-align -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wredundant-decls -Wmissing-format-attribute -Wformat-nonliteral")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG -march=native -Wall -Wextra -Wunused -Wshadow -Wredundant-decls -Wcast-align -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wredundant-decls -Wmissing-format-attribute -Wformat-nonliteral")


# -----------------------------------------------------------------------------
# Build Documentation
# -----------------------------------------------------------------------------
option(BUILD_DOC "Build documentation" OFF)
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
if(BUILD_TESTING)
    add_subdirectory(test)
endif()


include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
                            VERSION ${${PROJECT_NAME}_VERSION}
                            COMPATIBILITY AnyNewerVersion
                            VARS_PREFIX ${PROJECT_NAME}
                            NO_CHECK_REQUIRED_COMPONENTS_MACRO)
# Add the uninstall target
include(AddUninstallTarget)

include(LsDynatoApdlCPack)

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
message(STATUS "possible options: Debug Release")
message(STATUS "set with `cmake -D CMAKE_BUILD_TYPE=Debug ..`")
message(STATUS "Change a value with: cmake -D<Variable>=<Value> ..")
message(STATUS)
