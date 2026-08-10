# Warning flags for LsDynaToAPDL targets.
#
# Applied per target with target_compile_options rather than through an
# INTERFACE library: an INTERFACE library shows up in INTERFACE_LINK_LIBRARIES
# even when linked PRIVATE, so install(EXPORT) then demands that it be exported
# too, and consumers would inherit our warning flags. A plain function keeps
# the flags strictly local to the build.
#
# They are also deliberately not appended to CMAKE_CXX_FLAGS_*, which would
# apply them to every fetched dependency and drown the build in warnings from
# third-party code.

include_guard(GLOBAL)

function(lsdynatoapdl_apply_warnings target)
  if(MSVC)
    target_compile_options(${target} PRIVATE /W4 /utf-8 /EHsc)
  else()
    target_compile_options(
      ${target}
      PRIVATE -Wall
              -Wextra
              -Wunused
              -Wshadow
              -Wcast-align
              -Wmissing-include-dirs
              -Wnon-virtual-dtor
              -Woverloaded-virtual
              -Wformat-nonliteral)
  endif()

  if(LSDYNATOAPDL_WARNINGS_AS_ERRORS)
    if(MSVC)
      target_compile_options(${target} PRIVATE /WX)
    else()
      target_compile_options(${target} PRIVATE -Werror)
    endif()
  endif()
endfunction()

option(LSDYNATOAPDL_WARNINGS_AS_ERRORS "Treat compiler warnings as errors" OFF)
