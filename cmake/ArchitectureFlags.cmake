# Architecture-dependent compiler flags for LsDynaToAPDL.
#
# `-march=native` used to be applied unconditionally. That is wrong twice over:
# it is rejected outright by Apple Silicon toolchains (which spell it
# `-mcpu=apple-m1`) and by MSVC, and even where it is accepted it bakes the
# build machine's instruction set into the binary, so the result crashes with
# SIGILL on any older CPU — including most CI runners and release artefacts.
#
# Tuning is therefore opt-in, and every flag is probed before it is used.

include_guard(GLOBAL)
include(CheckCXXCompilerFlag)

option(LSDYNATOAPDL_ENABLE_NATIVE_ARCH
       "Optimise for the CPU of the build machine (not portable)" OFF)

# Normalise the many spellings of the same architecture into one token.
function(lsdynatoapdl_detect_architecture out_var)
  set(_raw "${CMAKE_SYSTEM_PROCESSOR}")
  if(NOT _raw)
    set(_raw "${CMAKE_HOST_SYSTEM_PROCESSOR}")
  endif()
  string(TOLOWER "${_raw}" _lower)

  if(_lower MATCHES "^(x86_64|amd64|x64)$")
    set(${out_var} "x86_64" PARENT_SCOPE)
  elseif(_lower MATCHES "^(aarch64|arm64|armv8)")
    set(${out_var} "arm64" PARENT_SCOPE)
  elseif(_lower MATCHES "^(armv7|armhf|arm)$")
    set(${out_var} "arm" PARENT_SCOPE)
  elseif(_lower MATCHES "^(riscv64)$")
    set(${out_var} "riscv64" PARENT_SCOPE)
  elseif(_lower MATCHES "^(ppc64le|powerpc64le)$")
    set(${out_var} "ppc64le" PARENT_SCOPE)
  elseif(_lower MATCHES "^(i386|i686|x86)$")
    set(${out_var} "x86" PARENT_SCOPE)
  else()
    set(${out_var} "${_lower}" PARENT_SCOPE)
  endif()
endfunction()

# Adds the native-tuning flag to `target`, if one exists and is supported.
function(lsdynatoapdl_apply_native_arch target)
  if(NOT LSDYNATOAPDL_ENABLE_NATIVE_ARCH)
    return()
  endif()

  if(MSVC)
    check_cxx_compiler_flag("/arch:AVX2" LSDYNATOAPDL_HAS_ARCH_AVX2)
    if(LSDYNATOAPDL_HAS_ARCH_AVX2)
      target_compile_options(${target} PRIVATE /arch:AVX2)
    endif()
    return()
  endif()

  # Apple Silicon and most AArch64 compilers use -mcpu; x86 uses -march.
  check_cxx_compiler_flag("-march=native" LSDYNATOAPDL_HAS_MARCH_NATIVE)
  if(LSDYNATOAPDL_HAS_MARCH_NATIVE)
    target_compile_options(${target} PRIVATE -march=native)
    return()
  endif()

  check_cxx_compiler_flag("-mcpu=native" LSDYNATOAPDL_HAS_MCPU_NATIVE)
  if(LSDYNATOAPDL_HAS_MCPU_NATIVE)
    target_compile_options(${target} PRIVATE -mcpu=native)
    return()
  endif()

  message(
    STATUS
      "No native tuning flag is supported by ${CMAKE_CXX_COMPILER_ID}; "
      "building without CPU-specific optimisation.")
endfunction()

lsdynatoapdl_detect_architecture(LSDYNATOAPDL_ARCHITECTURE)
message(
  STATUS
    "Target architecture: ${LSDYNATOAPDL_ARCHITECTURE} "
    "(${CMAKE_SYSTEM_NAME}, ${CMAKE_CXX_COMPILER_ID} ${CMAKE_CXX_COMPILER_VERSION})"
)
