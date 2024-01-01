
include( CMakeParseArguments )

execute_process(COMMAND git describe --tags --dirty
                OUTPUT_VARIABLE VERSION
                ERROR_QUIET
)

execute_process(COMMAND git rev-parse --short HEAD
                OUTPUT_VARIABLE VERSION_SHA1
                ERROR_QUIET
)

#parse the version information into pieces.
string(REGEX REPLACE "v?([0-9]+)\\..*" "\\1" VERSION_MAJOR "${VERSION}")
string(REGEX REPLACE "v?[0-9]+\\.([0-9]+).*" "\\1" VERSION_MINOR "${VERSION}")
string(REGEX REPLACE "v?[0-9]+\\.[0-9]+\\.([0-9]+).*" "\\1" VERSION_PATCH "${VERSION}")
string(REGEX REPLACE "\n$" "" VERSION_SHA1 "${VERSION_SHA1}")
set(VERSION_SHORT "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

message(STATUS "Git tag:       [${VERSION_SHORT}]")
message(STATUS "Git revision:  [${VERSION_SHA1}]")
