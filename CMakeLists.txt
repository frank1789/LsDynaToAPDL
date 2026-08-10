cmake_minimum_required(VERSION 3.30...4.3 FATAL_ERROR)

set(CMAKE_POLICY_DEFAULT_CMP0091 NEW)

# -----------------------------------------------------------------------------
# vcpkg
# -----------------------------------------------------------------------------
# Every third-party dependency comes from vcpkg, in manifest mode, driven by
# vcpkg.json. The toolchain file has to be selected before project(), so pick up
# the in-tree submodule unless the caller supplied a toolchain of their own —
# which the presets, and any distribution packager, do.
set(LSDYNATOAPDL_VCPKG_TOOLCHAIN
    "${CMAKE_CURRENT_SOURCE_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake")
if(NOT DEFINED CMAKE_TOOLCHAIN_FILE AND EXISTS "${LSDYNATOAPDL_VCPKG_TOOLCHAIN}")
    set(CMAKE_TOOLCHAIN_FILE
        "${LSDYNATOAPDL_VCPKG_TOOLCHAIN}"
        CACHE FILEPATH "vcpkg toolchain file")
    message(STATUS "Using the vcpkg submodule: ${CMAKE_TOOLCHAIN_FILE}")
elseif(NOT DEFINED CMAKE_TOOLCHAIN_FILE)
    message(
        FATAL_ERROR
            "No toolchain file and no vcpkg submodule.\n"
            "  Run: git submodule update --init --recursive\n"
            "  or configure with -DCMAKE_TOOLCHAIN_FILE=<your vcpkg>/scripts/buildsystems/vcpkg.cmake"
    )
endif()

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
option(BUILD_DOCUMENTATION "Build the documentation using Doxygen" OFF)

# -----------------------------------------------------------------------------
# load cmake module
# -----------------------------------------------------------------------------
include(GitVersion)
set(CMAKE_PROJECT_VERSION
    ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}
    CACHE STATIC "" FORCE)

# -----------------------------------------------------------------------------
# C/C++ standard
# -----------------------------------------------------------------------------
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# No C++20 modules are used. Scanning for them adds a pass to every build and
# feeds clang-tidy GCC-only flags (-fmodules-ts, -fdeps-format) that it rejects.
set(CMAKE_CXX_SCAN_FOR_MODULES OFF)

# -----------------------------------------------------------------------------
# Third-party dependencies and architecture handling
# -----------------------------------------------------------------------------
include(ArchitectureFlags)
include(CxxFeatures)

# Resolved by vcpkg from vcpkg.json. Keep that manifest and MODULE.bazel in
# step when a dependency is added or removed.
set(GFLAGS_USE_TARGET_NAMESPACE ON)
find_package(fmt CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(gflags CONFIG REQUIRED)
if(BUILD_TESTING)
    find_package(GTest CONFIG REQUIRED)
endif()

# Build test related commands
if(BUILD_TESTING)
    enable_testing()
    include(GoogleTest)
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

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)

# Find PThread Library
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
find_package(Threads REQUIRED)

# -----------------------------------------------------------------------------
# Per-target settings: warnings, coverage, sanitizers
# -----------------------------------------------------------------------------
include(ProjectTarget)
include(ClangTidy)

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
    lsdynatoapdl_add_coverage_report_target()
endif()

# -----------------------------------------------------------------------------
# Installed package files
# -----------------------------------------------------------------------------
# A single target consumers can link against instead of picking modules by
# hand. It also gives the generated config file the LsDynaToApdl::LsDynaToApdl
# target its compatibility block expects — without it find_package() failed
# with "get_property could not find TARGET LsDynaToApdl::LsDynaToApdl".
add_library(${PROJECT_NAME} INTERFACE)
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_link_libraries(
    ${PROJECT_NAME}
    INTERFACE ${PROJECT_NAME}::apdl ${PROJECT_NAME}::capi
              ${PROJECT_NAME}::common ${PROJECT_NAME}::logtools
              ${PROJECT_NAME}::lsdyna)
install(TARGETS ${PROJECT_NAME} EXPORT ${PROJECT_NAME})

include(InstallBasicPackageFiles)
# DEPENDENCIES becomes find_dependency() in the generated config. Without it a
# consumer's find_package(LsDynaToApdl) fails on the imported fmt::fmt and
# spdlog::spdlog targets the exported ones refer to.
install_basic_package_files(${PROJECT_NAME}
    VERSION   ${CMAKE_PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
    VARS_PREFIX   ${PROJECT_NAME}
    DEPENDENCIES  "fmt"
    PRIVATE_DEPENDENCIES "spdlog"
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# -----------------------------------------------------------------------------
# pkg-config
# -----------------------------------------------------------------------------
# For consumers that are not CMake projects — the C API is the whole point of
# having one. A static build has to advertise every archive and the
# LSDYNATOAPDL_STATIC_DEFINE macro, otherwise the export header decorates the
# symbols with dllimport.
set(LSDYNATOAPDL_PC_LIBS "-llsdynatoapdl_c")

# Requires.private is left empty on purpose. fmt and spdlog reach this build
# from vcpkg, the distro or FetchContent, and only some of those routes ship a
# .pc file; naming them here makes pkg-config fail the *entire* query — even
# --cflags — whenever fmt.pc is not on PKG_CONFIG_PATH. The private link line
# carries the same information without that failure mode. A distribution
# packager who knows the dependencies are pkg-config visible can override this.
set(LSDYNATOAPDL_PC_REQUIRES_PRIVATE
    ""
    CACHE STRING "Value of Requires.private in the generated .pc file")

if(BUILD_SHARED_LIBS)
    set(LSDYNATOAPDL_PC_LIBS_PRIVATE "")
    set(LSDYNATOAPDL_PC_CFLAGS
        "-DLSDYNATOAPDL_HAS_FLOAT_FROM_CHARS=${LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS}")
else()
    # A static consumer links the whole graph itself, and needs the export
    # macros to resolve to nothing rather than to dllimport.
    set(LSDYNATOAPDL_PC_LIBS_PRIVATE
        "-llsdynatoapdl_lsdyna -llsdynatoapdl_apdl -llsdynatoapdl_common -lspdlog -lfmt"
    )
    set(LSDYNATOAPDL_PC_CFLAGS
        "-DLSDYNATOAPDL_STATIC_DEFINE -DLSDYNATOAPDL_HAS_FLOAT_FROM_CHARS=${LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS}"
    )
endif()

# Walk back from <prefix>/<libdir>/pkgconfig to <prefix>; the depth varies
# between lib, lib64 and lib/<triplet>, so compute it rather than assume it.
file(RELATIVE_PATH LSDYNATOAPDL_PC_PREFIX_RELATIVE
     "${CMAKE_INSTALL_FULL_LIBDIR}/pkgconfig" "${CMAKE_INSTALL_PREFIX}")
string(REGEX REPLACE "/$" "" LSDYNATOAPDL_PC_PREFIX_RELATIVE
                     "${LSDYNATOAPDL_PC_PREFIX_RELATIVE}")

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/lsdynatoapdl.pc.in
               ${CMAKE_CURRENT_BINARY_DIR}/lsdynatoapdl.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/lsdynatoapdl.pc
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/pkgconfig"
        COMPONENT dev)

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
message(STATUS "Sanitizers:\t${LSDYNATOAPDL_SANITIZER}")
message(STATUS "Architecture:\t${LSDYNATOAPDL_ARCHITECTURE}")
message(STATUS "Documentation:\t${BUILD_DOCUMENTATION}")
message(STATUS "CMAKE_BINARY_DIR: ${CMAKE_BINARY_DIR}")
message(STATUS)
message(STATUS "possible options: Debug Release")
message(STATUS "set with `cmake -D CMAKE_BUILD_TYPE=Debug ..`")
message(STATUS "Change a value with: cmake -D<Variable>=<Value> ..")
message(STATUS)
