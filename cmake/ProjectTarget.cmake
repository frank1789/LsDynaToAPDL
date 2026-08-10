# One entry point for the per-target settings of this project.
#
# Warnings, coverage and sanitizers are all applied PRIVATE: they change how
# this project is compiled, and none of them belongs in the interface a
# consumer inherits. Keeping them behind a single call means a new module
# cannot accidentally opt out of half of them.

include_guard(GLOBAL)

include(CompilerWarnings)
include(Coverage)
include(Sanitizers)

function(lsdynatoapdl_configure_target target)
  lsdynatoapdl_apply_warnings(${target})
  lsdynatoapdl_apply_coverage(${target})
  lsdynatoapdl_apply_sanitizers(${target})
endfunction()
