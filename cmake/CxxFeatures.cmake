# Probes for standard-library features that are not uniformly available.
#
# Floating-point std::from_chars was the last part of <charconv> to be
# implemented. libstdc++ has had it since GCC 11 and MSVC since 19.24, but
# libc++ gained it late and Apple guards it with an availability attribute tied
# to the deployment target:
#
#   error: 'from_chars' is unavailable: introduced in macOS 26.0
#
# The declaration is present, so a feature-test macro alone can still say yes
# while the call fails to compile. Compiling the call is the only reliable
# answer, which is what this probe does — with the project's real flags and
# deployment target.

include_guard(GLOBAL)
include(CheckCXXSourceCompiles)

set(CMAKE_REQUIRED_QUIET TRUE)
set(_lsdynatoapdl_saved_cxx_standard "${CMAKE_REQUIRED_FLAGS}")

check_cxx_source_compiles(
  "
  #include <charconv>
  #include <system_error>
  int main() {
    const char* text = \"1.5\";
    double value{};
    const auto result = std::from_chars(text, text + 3, value);
    return result.ec == std::errc{} ? 0 : 1;
  }
  "
  LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS_PROBE)

set(CMAKE_REQUIRED_FLAGS "${_lsdynatoapdl_saved_cxx_standard}")
unset(CMAKE_REQUIRED_QUIET)

option(LSDYNATOAPDL_FORCE_FROM_CHARS_FALLBACK
       "Ignore floating-point std::from_chars even where it works" OFF)

if(LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS_PROBE AND NOT
                                               LSDYNATOAPDL_FORCE_FROM_CHARS_FALLBACK)
  set(LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 1)
  message(STATUS "std::from_chars(double): available")
else()
  set(LSDYNATOAPDL_HAS_FLOAT_FROM_CHARS 0)
  if(LSDYNATOAPDL_FORCE_FROM_CHARS_FALLBACK)
    message(STATUS "std::from_chars(double): fallback forced")
  else()
    message(
      STATUS "std::from_chars(double): unavailable, using the strtod_l fallback")
  endif()
endif()
