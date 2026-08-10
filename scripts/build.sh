#!/usr/bin/env sh
# Configure, build and test LsDynaToAPDL on the current platform.
#
# Uses the CMake preset that matches the host when one exists, and falls back
# to a plain Ninja configure everywhere else, so the same command works on an
# aarch64 laptop and on an x64 CI runner.
#
#   ./scripts/build.sh                 # release build and tests
#   ./scripts/build.sh --debug         # debug build
#   ./scripts/build.sh --no-tests      # skip ctest
#   ./scripts/build.sh --no-presets    # ignore presets, plain configure

set -eu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

BUILD_TYPE="Release"
RUN_TESTS=1
USE_PRESETS=1

while [ $# -gt 0 ]; do
    case "$1" in
        --debug) BUILD_TYPE="Debug" ;;
        --release) BUILD_TYPE="Release" ;;
        --no-tests) RUN_TESTS=0 ;;
        --no-presets) USE_PRESETS=0 ;;
        -h | --help)
            sed -n '2,12p' "$0" | sed 's/^# \{0,1\}//'
            exit 0
            ;;
        *)
            echo "unknown option: $1" >&2
            exit 2
            ;;
    esac
    shift
done

# shellcheck source=/dev/null
eval "$("${SCRIPT_DIR}/detect-platform.sh" --export)"

echo "==> ${LSDYNATOAPDL_OS}/${LSDYNATOAPDL_ARCH}, ${BUILD_TYPE}"

# Every dependency comes from vcpkg, so the submodule has to be there either
# way; the presets simply pin the triplet as well.
if [ ! -f "${PROJECT_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
    echo "==> vcpkg submodule missing" >&2
    echo "    run: git submodule update --init --recursive" >&2
    exit 1
fi

if [ "${USE_PRESETS}" -eq 1 ] && [ -n "${LSDYNATOAPDL_PRESET}" ] &&
    [ -f "${PROJECT_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" ] &&
    [ "${BUILD_TYPE}" = "Release" ]; then
    echo "==> preset ${LSDYNATOAPDL_PRESET}"
    cmake --preset "${LSDYNATOAPDL_PRESET}"
    cmake --build --preset "${LSDYNATOAPDL_PRESET}"
    if [ "${RUN_TESTS}" -eq 1 ]; then
        ctest --preset "${LSDYNATOAPDL_PRESET}"
    fi
    exit 0
fi

if [ "${USE_PRESETS}" -eq 1 ] && [ -n "${LSDYNATOAPDL_PRESET}" ] &&
    [ ! -f "${PROJECT_DIR}/vcpkg/scripts/buildsystems/vcpkg.cmake" ]; then
    echo "==> vcpkg submodule missing; dependencies come from vcpkg only"
    echo "    run: git submodule update --init --recursive"
    exit 1
fi

BUILD_DIR="${PROJECT_DIR}/build/${LSDYNATOAPDL_OS}-${LSDYNATOAPDL_ARCH}-$(echo "${BUILD_TYPE}" | tr '[:upper:]' '[:lower:]')"
GENERATOR="Unix Makefiles"
if command -v ninja >/dev/null 2>&1; then
    GENERATOR="Ninja"
fi

echo "==> configure ${BUILD_DIR} (${GENERATOR})"
cmake -S "${PROJECT_DIR}" -B "${BUILD_DIR}" -G "${GENERATOR}" \
    -DCMAKE_BUILD_TYPE="${BUILD_TYPE}"

echo "==> build"
cmake --build "${BUILD_DIR}" --parallel

if [ "${RUN_TESTS}" -eq 1 ]; then
    echo "==> test"
    ctest --test-dir "${BUILD_DIR}" --output-on-failure
fi

echo "==> done: ${BUILD_DIR}"
