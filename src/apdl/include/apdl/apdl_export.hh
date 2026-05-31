#ifndef LSDYNA_TO_APDL_EXPORT_HH_
#define LSDYNA_TO_APDL_EXPORT_HH_

#if defined(_WIN32) || defined(__CYGWIN__)
#  ifdef LIBRARY_EXPORTS
#    define LIBRARY_API __declspec(dllexport)
#  else
#    define LIBRARY_API __declspec(dllimport)
#  endif
#elif defined(__GNUC__) && __GNUC__ >= 4
#  define LIBRARY_API __attribute__((visibility("default")))
#endif

#endif  // LSDYNA_TO_APDL_EXPORT_HH_
