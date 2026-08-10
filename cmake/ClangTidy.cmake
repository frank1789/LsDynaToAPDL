# clang-tidy integration.
#
# Off by default: it roughly doubles compile time, which is not what anyone
# wants from an ordinary build. CI and anyone auditing style switch it on
# explicitly. The checks themselves live in .clang-tidy at the repository root.
#
# Third-party code is never analysed. Two things keep it out, because either
# one alone is breakable:
#
#   * dependencies are included with -isystem, and clang-tidy skips system
#     headers — but that only holds while every dependency arrives as an
#     IMPORTED target, which CMake marks SYSTEM by default;
#   * the header filter is anchored to this project's absolute source
#     directory, and third-party trees are excluded by name.
#
# The second matters more than it looks: a relative pattern such as
# "src/.*\.(hh|h)$" is unanchored, so it also matches
# vcpkg/buildtrees/fmt/src/.../format.h and googletest/googletest/src/...

include_guard(GLOBAL)

option(LSDYNATOAPDL_ENABLE_CLANG_TIDY "Run clang-tidy as part of the build" OFF)

if(NOT LSDYNATOAPDL_ENABLE_CLANG_TIDY)
  return()
endif()

find_program(CLANG_TIDY_EXECUTABLE NAMES clang-tidy)
if(NOT CLANG_TIDY_EXECUTABLE)
  message(FATAL_ERROR "LSDYNATOAPDL_ENABLE_CLANG_TIDY=ON but clang-tidy was "
                      "not found on PATH.")
endif()

# Escape the regex metacharacters a checkout path may legitimately contain.
function(_lsdynatoapdl_regex_escape input output)
  string(REGEX REPLACE "([][().*+?^$|\\\\])" "\\\\\\1" _escaped "${input}")
  set(${output} "${_escaped}" PARENT_SCOPE)
endfunction()

_lsdynatoapdl_regex_escape("${CMAKE_SOURCE_DIR}" _lsdynatoapdl_source_re)
_lsdynatoapdl_regex_escape("${CMAKE_BINARY_DIR}" _lsdynatoapdl_binary_re)

# Only this project's own headers, addressed absolutely so nothing outside the
# checkout can match.
set(LSDYNATOAPDL_TIDY_HEADER_FILTER
    "^${_lsdynatoapdl_source_re}/(src|test)/.*\\.(hh|h)$")

# Belt and braces: the vcpkg submodule and the build tree sit inside the source
# directory, and vcpkg unpacks ports into vcpkg/buildtrees/<port>/src/.
set(LSDYNATOAPDL_TIDY_EXCLUDE_FILTER
    "^(${_lsdynatoapdl_source_re}/vcpkg/|${_lsdynatoapdl_binary_re}/)")

set(_lsdynatoapdl_tidy_command "${CLANG_TIDY_EXECUTABLE}" "--quiet"
                               "--header-filter=${LSDYNATOAPDL_TIDY_HEADER_FILTER}")

# --exclude-header-filter arrived in clang-tidy 19; older versions reject it.
execute_process(
  COMMAND "${CLANG_TIDY_EXECUTABLE}" --help
  OUTPUT_VARIABLE _lsdynatoapdl_tidy_help
  ERROR_VARIABLE _lsdynatoapdl_tidy_help
  OUTPUT_STRIP_TRAILING_WHITESPACE)
if(_lsdynatoapdl_tidy_help MATCHES "exclude-header-filter")
  list(APPEND _lsdynatoapdl_tidy_command
       "--exclude-header-filter=${LSDYNATOAPDL_TIDY_EXCLUDE_FILTER}")
else()
  message(
    STATUS
      "clang-tidy has no --exclude-header-filter; the anchored header filter "
      "is doing the work on its own.")
endif()

set(CMAKE_CXX_CLANG_TIDY
    ${_lsdynatoapdl_tidy_command}
    CACHE STRING "clang-tidy command line" FORCE)

message(STATUS "clang-tidy: ${CLANG_TIDY_EXECUTABLE} (project headers only)")
