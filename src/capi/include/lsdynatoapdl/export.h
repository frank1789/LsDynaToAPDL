/**
 * @file export.h
 * @author Francesco Argentieri (francesco.argentieri89@gmail.com)
 * @brief Symbol visibility macros for the LsDynaToAPDL shared library.
 * @version 0.1.0
 * @date 2026-08-10
 *
 * @copyright Copyright (c) 2026
 *
 */

#ifndef LSDYNA_TO_APDL_EXPORT_H
#define LSDYNA_TO_APDL_EXPORT_H

/*
 * Three build situations have to be told apart, and the previous version of
 * this header handled only two:
 *
 *   - building the shared library      -> dllexport / default visibility
 *   - consuming the shared library     -> dllimport
 *   - building or consuming statically -> nothing at all
 *
 * Without the third case a static build on Windows is decorated with
 * dllimport and fails to link. LSDYNATOAPDL_STATIC_DEFINE, set by CMake for
 * static builds, selects it.
 *
 * The macros are also prefixed. The old LIBRARY_API / LIBRARY_EXPORTS names
 * are generic enough to collide with any other library a consumer happens to
 * include.
 */

#ifdef LSDYNATOAPDL_STATIC_DEFINE
#  define LSDYNATOAPDL_API
#  define LSDYNATOAPDL_LOCAL
#elif defined(_WIN32) || defined(__CYGWIN__)
#  ifdef LSDYNATOAPDL_EXPORTS
#    define LSDYNATOAPDL_API __declspec(dllexport)
#  else
#    define LSDYNATOAPDL_API __declspec(dllimport)
#  endif
#  define LSDYNATOAPDL_LOCAL
#elif defined(__GNUC__) || defined(__clang__)
#  define LSDYNATOAPDL_API __attribute__((visibility("default")))
#  define LSDYNATOAPDL_LOCAL __attribute__((visibility("hidden")))
#else
/* Unknown compiler: exporting everything is the safe default. */
#  define LSDYNATOAPDL_API
#  define LSDYNATOAPDL_LOCAL
#endif

#endif /* LSDYNA_TO_APDL_EXPORT_H */
