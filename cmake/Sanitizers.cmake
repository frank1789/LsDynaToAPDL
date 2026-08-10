# Runtime sanitizers for LsDynaToAPDL.
#
# Driven by one cache variable so the presets stay short:
#
#   -DLSDYNATOAPDL_SANITIZER=address
#   -DLSDYNATOAPDL_SANITIZER="address;undefined"
#
# Support differs per compiler and some combinations are mutually exclusive
# (thread cannot be combined with address), so the request is validated here
# and rejected with a clear message rather than failing deep inside the link.
#
# Flags are attached per target with PRIVATE visibility. A sanitizer changes
# the ABI of the code it instruments, so it must never leak to a consumer
# through INTERFACE properties: mixing an instrumented library with an
# uninstrumented consumer is exactly how one gets an unexplainable crash.

include_guard(GLOBAL)

set(LSDYNATOAPDL_SANITIZER
    ""
    CACHE STRING
          "Sanitizers to enable: address, thread, undefined, leak, memory")
set_property(CACHE LSDYNATOAPDL_SANITIZER
             PROPERTY STRINGS "" address undefined thread leak memory
                      "address;undefined")

# Fills LSDYNATOAPDL_SANITIZER_FLAGS, or errors out when the request cannot be
# honoured by this compiler.
function(_lsdynatoapdl_resolve_sanitizers out_flags)
  set(_requested "${LSDYNATOAPDL_SANITIZER}")
  string(REPLACE "," ";" _requested "${_requested}")
  list(REMOVE_DUPLICATES _requested)
  list(REMOVE_ITEM _requested "")

  if(NOT _requested)
    set(${out_flags} "" PARENT_SCOPE)
    return()
  endif()

  # thread instruments memory accesses in a way that is incompatible with the
  # address/leak shadow memory; enabling both produces a runtime abort.
  if("thread" IN_LIST _requested)
    foreach(_conflict address leak memory)
      if("${_conflict}" IN_LIST _requested)
        message(
          FATAL_ERROR
            "LSDYNATOAPDL_SANITIZER: 'thread' cannot be combined with "
            "'${_conflict}'. Configure two build directories instead.")
      endif()
    endforeach()
  endif()

  set(_flags "")

  if(MSVC)
    # MSVC implements AddressSanitizer only; there is no UBSan or TSan.
    foreach(_sanitizer IN LISTS _requested)
      if(NOT _sanitizer STREQUAL "address")
        message(
          FATAL_ERROR
            "LSDYNATOAPDL_SANITIZER: MSVC supports only 'address', not "
            "'${_sanitizer}'. Use clang-cl for the others.")
      endif()
    endforeach()
    set(_flags /fsanitize=address)
  else()
    if("memory" IN_LIST _requested AND NOT CMAKE_CXX_COMPILER_ID MATCHES
                                       "Clang")
      message(
        FATAL_ERROR
          "LSDYNATOAPDL_SANITIZER: 'memory' is Clang only, and needs an "
          "instrumented standard library.")
    endif()

    list(JOIN _requested "," _joined)
    set(_flags -fsanitize=${_joined} -fno-omit-frame-pointer
               -fno-optimize-sibling-calls)

    # Symbolised, actionable reports are the whole point of a sanitizer run.
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
      list(APPEND _flags -g)
    endif()
  endif()

  set(${out_flags} "${_flags}" PARENT_SCOPE)
endfunction()

_lsdynatoapdl_resolve_sanitizers(LSDYNATOAPDL_SANITIZER_FLAGS)

if(LSDYNATOAPDL_SANITIZER_FLAGS)
  message(STATUS "Sanitizers: ${LSDYNATOAPDL_SANITIZER}")
endif()

# Instrument `target`, if any sanitizer was requested.
function(lsdynatoapdl_apply_sanitizers target)
  if(NOT LSDYNATOAPDL_SANITIZER_FLAGS)
    return()
  endif()

  target_compile_options(${target} PRIVATE ${LSDYNATOAPDL_SANITIZER_FLAGS})
  if(NOT MSVC)
    # The runtime has to be linked too, and the flag belongs on the link line
    # of every instrumented target, not only the final executable.
    target_link_options(${target} PRIVATE ${LSDYNATOAPDL_SANITIZER_FLAGS})
  endif()
endfunction()
