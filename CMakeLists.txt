cmake_minimum_required(VERSION 3.30...4.3 FATAL_ERROR)

set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

project(
  LsDynaToApdl
  LANGUAGES CXX
  VERSION 0.0.0
)

# append additional scripts
list(APPEND CMAKE_MODULE_PATH ${LsDynaToApdl_SOURCE_DIR}/cmake)

# Control where libraries and executables are placed during the build.
include(GNUInstallDirs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# To build shared libraries in Windows, we set CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS
# to TRUE.
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# Under MSVC, we set CMAKE_DEBUG_POSTFIX to "d" to add a trailing "d" to library
# built in debug mode. In this Windows user can compile, build and install the
# library in both Release and Debug configuration avoiding naming clashes in the
# installation directories.
if(MSVC)
    set(CMAKE_DEBUG_POSTFIX "d")
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" OFF)
option(BUILD_TESTING "Create tests using CMake" ON)
option(COVERAGE "Build with code coverage enabled" OFF)
option(BUILD_DOCUMENTATION "Build the documentation using Doxygen" OFF)

# -----------------------------------------------------------------------------
# load cmake module
# -----------------------------------------------------------------------------
include(GitVersion)
set(CMAKE_PROJECT_VERSION
    ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    CACHE STATIC "" FORCE)

# Build test related commands
if(BUILD_TESTING)
    enable_testing()
    find_package(GTest CONFIG REQUIRED)
    include(GoogleTest)

    # ---------------------------------------------------------------------------
    # Code coverage
    # ---------------------------------------------------------------------------
    if(COVERAGE)
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
add_install_rpath_support(
    BIN_DIRS            "${CMAKE_INSTALL_FULL_BINDIR}"
    LIB_DIRS            "${CMAKE_INSTALL_FULL_LIBDIR}"
    INSTALL_NAME_DIR    "${CMAKE_INSTALL_FULL_LIBDIR}"
    USE_LINK_PATH
)

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise
# set it to Release.
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
        message(STATUS "Setting build type to 'Release' as none was specified.")
        set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
    endif()
endif()

# -----------------------------------------------------------------------------
# C/C++ standard extensions
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find PThread Library
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# -----------------------------------------------------------------------------
# Additional libraries
# -----------------------------------------------------------------------------
find_package(spdlog CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)

set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(gflags CONFIG REQUIRED)

if(NOT WIN32)
    set(CMAKE_CXX_FLAGS_DEBUG
      "${CMAKE_CXX_FLAGS_DEBUG} -O0 -march=native -Wall -Wextra -Wunused -Wshadow -Wredundant-decls -Wcast-align -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wredundant-decls -Wmissing-format-attribute -Wformat-nonliteral"
  )
    set(CMAKE_CXX_FLAGS_RELEASE
      "-O3 -DNDEBUG -march=native -Wall -Wextra -Wunused -Wshadow -Wredundant-decls -Wcast-align -Wmissing-declarations -Wmissing-include-dirs -Winvalid-pch -Wredundant-decls -Wmissing-format-attribute -Wformat-nonliteral"
  )
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /nologo /EHsc /Zi /W4 /utf-8 /arch:AVX2")
    set(CMAKE_CXX_FLAGS_RELEASE "/nologo /EHsc /W4 /utf-8 /O2 /Ot /arch:AVX2")
    if(MSVC)
        add_compile_options($<$<CONFIG:>:/MT> $<$<CONFIG:Debug>:/MTd> $<$<CONFIG:Release>:/MT>)
    endif()
endif()

# -----------------------------------------------------------------------------
# Build Documentation
# -----------------------------------------------------------------------------
if(BUILD_DOCUMENTATION)
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
    else()
        message(
      STATUS "Doxygen need to be installed to generate the doxygen documentation")
    endif()
endif()

# -----------------------------------------------------------------------------
# Local library and dependencies
# -----------------------------------------------------------------------------
add_subdirectory(src)
if(BUILD_TESTING)
    add_subdirectory(test)
endif()

include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
    VERSION   ${${PROJECT_NAME}_VERSION}
    COMPATIBILITY AnyNewerVersion
    VARS_PREFIX   ${PROJECT_NAME}
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

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
message(STATUS "Documentation:\t${BUILD_DOCUMENTATION}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS)
message(STATUS "possible options: Debug Release")
message(STATUS "set with `cmake -D CMAKE_BUILD_TYPE=Debug ..`")
message(STATUS "Change a value with: cmake -D<Variable>=<Value> ..")
message(STATUS)
