#!/usr/bin/env sh
# Verify that this machine can build LsDynaToAPDL before a build is attempted.
#
# Checks the tools and, most importantly, that the C++ compiler really accepts
# C++23 — a compiler can advertise the flag and still be missing the library
# pieces the project uses. A five-second compile here saves a confusing wall
# of template errors later.
#
#   ./scripts/check-toolchain.sh

set -eu

STATUS=0
CXX="${CXX:-c++}"

say_ok() { printf '  ok      %s\n' "$1"; }
say_warn() { printf '  warn    %s\n' "$1"; }
say_fail() {
    printf '  FAIL    %s\n' "$1"
    STATUS=1
}

require_tool() {
    if command -v "$1" >/dev/null 2>&1; then
        say_ok "$1 ($($1 --version 2>&1 | head -n 1))"
    else
        say_fail "$1 not found: $2"
    fi
}

optional_tool() {
    if command -v "$1" >/dev/null 2>&1; then
        say_ok "$1"
    else
        say_warn "$1 not found: $2"
    fi
}

echo "platform"
printf '  os      %s\n' "$(uname -s)"
printf '  arch    %s\n' "$(uname -m)"

echo "required tools"
require_tool cmake "install CMake 3.30 or newer"
require_tool "${CXX}" "install a C++23 compiler (GCC 13+, Clang 16+)"

echo "optional tools"
optional_tool ninja "faster builds; any generator works"
optional_tool git "needed for the version string and the vcpkg submodule"
optional_tool bazel "only for the optional Bazel build"
optional_tool doxygen "only for -DBUILD_DOCUMENTATION=ON"
optional_tool pre-commit "commit hooks; see CONTRIBUTING"

echo "compiler capabilities"
TMPDIR_CHECK="$(mktemp -d)"
trap 'rm -rf "${TMPDIR_CHECK}"' EXIT

cat >"${TMPDIR_CHECK}/probe.cc" <<'PROBE'
// Exercises exactly the C++23 surface the project relies on.
#include <charconv>
#include <concepts>
#include <filesystem>
#include <optional>
#include <string_view>
#include <system_error>

template <typename Derived>
struct Base {
  [[nodiscard]] auto text() const -> std::string_view {
    return static_cast<const Derived&>(*this).value();
  }
};

struct Derived : Base<Derived> {
  [[nodiscard]] auto value() const -> std::string_view { return "ok"; }
};

template <typename T>
concept Printable = std::derived_from<T, Base<T>>;

static_assert(Printable<Derived>);

int main() {
  double parsed{};
  const std::string_view input{"1.5"};
  const auto result =
      std::from_chars(input.data(), input.data() + input.size(), parsed);
  const std::optional<double> value =
      result.ec == std::errc{} ? std::optional{parsed} : std::nullopt;
  return (value && *value == 1.5 && Derived{}.text() == "ok") ? 0 : 1;
}
PROBE

if "${CXX}" -std=c++23 -o "${TMPDIR_CHECK}/probe" "${TMPDIR_CHECK}/probe.cc" \
    >"${TMPDIR_CHECK}/probe.log" 2>&1; then
    say_ok "${CXX} compiles C++23 (concepts, from_chars, filesystem)"
    if "${TMPDIR_CHECK}/probe"; then
        say_ok "the probe runs correctly"
    else
        say_fail "the probe compiled but did not behave as expected"
    fi
else
    say_fail "${CXX} cannot compile the C++23 probe; see the output below"
    sed 's/^/          /' "${TMPDIR_CHECK}/probe.log"
fi

echo
if [ "${STATUS}" -eq 0 ]; then
    echo "toolchain looks good"
else
    echo "toolchain is missing something; see the FAIL lines above"
fi
exit "${STATUS}"
