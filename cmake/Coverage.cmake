# Code coverage for LsDynaToAPDL.
#
# The previous implementation was inert: it created an INTERFACE target called
# `coverage` that no target ever linked, so -DCOVERAGE=ON produced a normal,
# uninstrumented build and an empty report. It also dereferenced
# ${CMAKE_BUILD_TYPE} unquoted, which is a hard error under a multi-config
# generator where the variable is empty.
#
# This version attaches the flags per target, PRIVATE, and adds a
# `coverage-report` target that runs gcovr with the right gcov backend for the
# compiler in use.
#
#   cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug -DCOVERAGE=ON
#   cmake --build build --target coverage-report

include_guard(GLOBAL)

option(COVERAGE "Instrument the build to collect code coverage" OFF)

set(LSDYNATOAPDL_COVERAGE_FLAGS "")

if(COVERAGE)
  if(MSVC)
    message(
      FATAL_ERROR
        "COVERAGE is not supported with MSVC; use OpenCppCoverage on the "
        "uninstrumented binaries, or build with clang-cl.")
  elseif(NOT CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    message(FATAL_ERROR "COVERAGE needs GCC or Clang, found "
                        "${CMAKE_CXX_COMPILER_ID}.")
  endif()

  # Optimised code is inlined and reordered, so line counters stop matching the
  # source. Coverage numbers from a Release build are not meaningful.
  if(NOT "${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
    message(
      WARNING
        "COVERAGE=ON with CMAKE_BUILD_TYPE='${CMAKE_BUILD_TYPE}'. Optimised "
        "builds inline and reorder code, so the report will not line up with "
        "the sources. Configure with -DCMAKE_BUILD_TYPE=Debug.")
  endif()

  # --coverage implies -fprofile-arcs -ftest-coverage when compiling and links
  # the profiling runtime. Clang accepts it too and emits gcov-compatible data.
  set(LSDYNATOAPDL_COVERAGE_FLAGS --coverage -O0 -g -fno-inline
                                  -fno-elide-constructors)

  message(STATUS "Coverage: enabled (${CMAKE_CXX_COMPILER_ID})")
endif()

# Instrument `target` when coverage is enabled.
function(lsdynatoapdl_apply_coverage target)
  if(NOT COVERAGE)
    return()
  endif()
  target_compile_options(${target} PRIVATE ${LSDYNATOAPDL_COVERAGE_FLAGS})
  target_link_options(${target} PRIVATE --coverage)
endfunction()

# Adds the `coverage-report` target once, if a report generator is available.
function(lsdynatoapdl_add_coverage_report_target)
  if(NOT COVERAGE OR TARGET coverage-report)
    return()
  endif()

  find_program(GCOVR_EXECUTABLE gcovr)
  if(NOT GCOVR_EXECUTABLE)
    message(
      STATUS
        "gcovr not found: raw .gcda data will still be produced, but the "
        "coverage-report target is unavailable.")
    return()
  endif()

  # Clang writes gcov data in its own format; `llvm-cov gcov` reads it.
  if(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    find_program(LLVM_COV_EXECUTABLE llvm-cov)
    if(LLVM_COV_EXECUTABLE)
      set(_gcov_arg --gcov-executable "${LLVM_COV_EXECUTABLE} gcov")
    else()
      message(STATUS "llvm-cov not found; gcovr may fail to read Clang data.")
      set(_gcov_arg "")
    endif()
  else()
    set(_gcov_arg "")
  endif()

  add_custom_target(
    coverage-report
    COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure
    COMMAND ${CMAKE_COMMAND} -E make_directory
            "${CMAKE_BINARY_DIR}/coverage"
    COMMAND
      ${GCOVR_EXECUTABLE} --root "${CMAKE_SOURCE_DIR}" ${_gcov_arg} --exclude
      "${CMAKE_SOURCE_DIR}/test" --exclude "${CMAKE_SOURCE_DIR}/vcpkg"
      --exclude "${CMAKE_BINARY_DIR}" --print-summary --html-details
      "${CMAKE_BINARY_DIR}/coverage/index.html" --xml
      "${CMAKE_BINARY_DIR}/coverage/coverage.xml"
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
    COMMENT "Running the tests and collecting coverage"
    VERBATIM)
endfunction()
