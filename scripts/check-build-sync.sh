#!/usr/bin/env sh
# Check that every source and header on disk is listed in both build systems.
#
# CMake and Bazel each enumerate their files explicitly, so adding a file means
# editing two lists. Forgetting the Bazel one is invisible locally — bazel is
# not always installed — and only surfaces in CI. This catches it in a second.
#
#   ./scripts/check-build-sync.sh

set -eu

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "${SCRIPT_DIR}/.." && pwd)"

python3 - "${PROJECT_DIR}" <<'PY'
import pathlib
import re
import sys

root = pathlib.Path(sys.argv[1])
status = 0

for module in sorted(p for p in (root / "src").iterdir() if p.is_dir()):
    on_disk = {
        str(f.relative_to(module))
        for f in module.rglob("*")
        if f.is_file() and f.suffix in {".cc", ".hh", ".h"}
    }
    if not on_disk:
        continue

    for build_file, pattern in (
        ("BUILD.bazel", r'"((?:src|include)/[^"]+)"'),
        ("CMakeLists.txt", r'\$\{CMAKE_CURRENT_SOURCE_DIR\}/((?:src|include)/[^\s)]+)'),
    ):
        path = module / build_file
        if not path.exists():
            continue
        listed = set(re.findall(pattern, path.read_text()))
        missing = sorted(on_disk - listed)
        stale = sorted(f for f in listed - on_disk)
        if missing:
            print(f"  {module.name}/{build_file}: not listed -> {', '.join(missing)}")
            status = 1
        if stale:
            print(f"  {module.name}/{build_file}: listed but absent -> {', '.join(stale)}")
            status = 1

print("build files are in sync" if status == 0 else "build files are OUT OF SYNC")
sys.exit(status)
PY
