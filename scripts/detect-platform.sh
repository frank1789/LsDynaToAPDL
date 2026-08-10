#!/usr/bin/env sh
# Detect the host platform and print the matching CMake presets.
#
# Sourcing this script exports LSDYNATOAPDL_OS, LSDYNATOAPDL_ARCH and the
# three preset names; running it prints them as shell assignments, so both
# CI and a human can consume it:
#
#   ./scripts/detect-platform.sh              # human readable
#   eval "$(./scripts/detect-platform.sh --export)"
#
# POSIX sh on purpose: this has to run on a minimal container, on a BSD, and
# on Git Bash under Windows, none of which are guaranteed to have bash.

set -eu

detect_os() {
    case "$(uname -s)" in
        Linux*) echo "linux" ;;
        Darwin*) echo "macos" ;;
        CYGWIN* | MINGW* | MSYS* | Windows_NT) echo "windows" ;;
        FreeBSD*) echo "freebsd" ;;
        *) echo "unknown" ;;
    esac
}

detect_arch() {
    # uname -m spells the same architecture several ways depending on the OS,
    # so normalise to the tokens used by the presets and by vcpkg triplets.
    case "$(uname -m)" in
        x86_64 | amd64) echo "x64" ;;
        aarch64 | arm64) echo "arm64" ;;
        armv7* | armv6*) echo "arm" ;;
        riscv64) echo "riscv64" ;;
        ppc64le) echo "ppc64le" ;;
        i386 | i686) echo "x86" ;;
        *) uname -m ;;
    esac
}

LSDYNATOAPDL_OS="$(detect_os)"
LSDYNATOAPDL_ARCH="$(detect_arch)"

# Presets are <arch>-<os>-<configuration>, and the configure, build and test
# preset of a configuration share one name. x64 and arm64 are covered; anything
# else has no vcpkg triplet here, and the caller configures without a preset.
case "${LSDYNATOAPDL_ARCH}" in
    x64 | arm64)
        case "${LSDYNATOAPDL_OS}" in
            linux | macos | windows)
                LSDYNATOAPDL_PRESET="${LSDYNATOAPDL_ARCH}-${LSDYNATOAPDL_OS}-release"
                ;;
            *) LSDYNATOAPDL_PRESET="" ;;
        esac
        ;;
    *) LSDYNATOAPDL_PRESET="" ;;
esac

if [ "${1:-}" = "--export" ]; then
    echo "export LSDYNATOAPDL_OS='${LSDYNATOAPDL_OS}'"
    echo "export LSDYNATOAPDL_ARCH='${LSDYNATOAPDL_ARCH}'"
    echo "export LSDYNATOAPDL_PRESET='${LSDYNATOAPDL_PRESET}'"
else
    echo "os:        ${LSDYNATOAPDL_OS}"
    echo "arch:      ${LSDYNATOAPDL_ARCH}"
    if [ -n "${LSDYNATOAPDL_PRESET}" ]; then
        echo "preset:    ${LSDYNATOAPDL_PRESET}"
    else
        echo "preset:    (none for this platform, plain Ninja configure)"
    fi
fi
