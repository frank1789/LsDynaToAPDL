# LsDynaToAPDL

Convert LS-DYNA keyword decks (`*.k`) into Ansys **APDL** input files.

- [LsDynaToAPDL](#lsdynatoapdl)
  - [What it does](#what-it-does)
  - [Requirements](#requirements)
  - [Configure and build](#configure-and-build)
  - [Usage](#usage)
  - [Output format](#output-format)
  - [Library and C API](#library-and-c-api)
  - [Architecture](#architecture)
  - [Testing](#testing)
  - [Motivation](#motivation)
  - [Contributing](#contributing)
  - [Authors](#authors)
  - [License](#license)

[![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0)
[![C++23](https://img.shields.io/badge/C%2B%2B-23-blue.svg)](https://en.cppreference.com/w/cpp/23)

[![CI](https://github.com/frank1789/LsDynaToAPDL/actions/workflows/ci.yaml/badge.svg)](https://github.com/frank1789/LsDynaToAPDL/actions/workflows/ci.yaml)

## What it does

The converter reads the sections of a LS-DYNA deck that have a counterpart in
APDL and rewrites them as preprocessor commands.

| LS-DYNA section | APDL output | Status |
|:----------------|:------------|:-------|
| `*NODE` | `N, NODE, X, Y, Z` | supported |
| `*ELEMENT_SHELL_THICKNESS` | `EN` + `SECTYPE`/`SECDATA` as **SHELL181** | supported |
| keypoints (through the API) | `K, NPT, X, Y, Z` | supported |
| `*ELEMENT_SOLID` | — | ignored |
| `*INITIAL_STRESS_SHELL`, `*INITIAL_STRAIN_SOLID` | — | ignored |

Shell elements are written as `SHELL181`. A LS-DYNA triangle — a quadrilateral
whose last two nodes coincide — is passed through unchanged, which is how
SHELL181 expects a degenerate element.

## Requirements

- **CMake ≥ 3.30** and a **C++23** compiler (GCC 13+, Clang 16+, MSVC 19.36+,
  Apple Clang 15+ on macOS Ventura and newer)
- fmt, spdlog, gflags and GoogleTest

Dependencies come from **vcpkg**, in manifest mode, driven by `vcpkg.json`.
The vcpkg submodule ships with the repository, so nothing has to be installed
by hand:

```sh
git submodule update --init --recursive
```

The presets select the toolchain file; a plain `cmake -S . -B build` picks up
the same submodule automatically. Point `CMAKE_TOOLCHAIN_FILE` at your own
vcpkg to use that instead.

Check the machine before building:

```sh
./scripts/check-toolchain.sh
```

## Configure and build

The quickest path, on any supported platform and architecture:

```sh
./scripts/build.sh              # configure, build and run the tests
./scripts/build.sh --debug      # debug build
./scripts/build.sh --no-tests   # skip ctest
```

It picks the CMake preset matching the host and falls back to a plain Ninja
configure where no preset exists (for example on aarch64).

<details>
<summary>Doing it by hand</summary>

With the vcpkg submodule, using the presets:

```sh
git submodule update --init --recursive
cmake --preset x64-linux-release
cmake --build --preset x64-linux-release
ctest --preset x64-linux-release
```

Presets are named `x64-<os>-<configuration>`, and the configure, build and test
preset of a configuration share one name. The macOS presets keep the name for
consistency but build for the **host** architecture, so they produce arm64
binaries on Apple Silicon rather than an x86_64 build that would not link. `<os>` is `linux`, `macos` or
`windows`; `<configuration>` is `debug`, `release`, `relwithdebinfo`, a
sanitizer (`address`, `undefined`, `thread`) or `coverage`. Each platform uses
its native compiler — MSVC, AppleClang, GCC — and `x64-linux-clang-release`
and `x64-windows-clang-release` select Clang and clang-cl instead.

Without vcpkg, letting CMake fetch what is missing:

```sh
cmake -S . -B build -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Useful options:

| Option | Default | Meaning |
|:-------|:--------|:--------|
| `LSDYNATOAPDL_ENABLE_NATIVE_ARCH` | `OFF` | tune for the build machine's CPU (not portable) |
| `LSDYNATOAPDL_SANITIZER` | *(empty)* | `address`, `undefined`, `thread`, `leak`, `memory` |
| `LSDYNATOAPDL_WARNINGS_AS_ERRORS` | `OFF` | turn warnings into errors |
| `LSDYNATOAPDL_ENABLE_CLANG_TIDY` | `OFF` | run clang-tidy during the build |
| `LSDYNATOAPDL_FORCE_FROM_CHARS_FALLBACK` | `OFF` | use the strtod_l number parser even where `std::from_chars` works |
| `BUILD_SHARED_LIBS` | `OFF` | build shared libraries |
| `BUILD_TESTING` | `ON` | build the test suite |
| `BUILD_DOCUMENTATION` | `OFF` | generate the Doxygen documentation |
| `COVERAGE` | `OFF` | instrument a Debug build for coverage |

Sanitizers and coverage are validated against the compiler at configure time,
so an unsupported request fails immediately with an explanation instead of
part-way through the link:

```sh
cmake --preset x64-linux-address     # ASan + LSan
cmake --preset x64-linux-coverage    # then: cmake --build ... --target coverage-report
```

Bazel is supported as an optional second build system:

```sh
bazel build //... -c opt
bazel test  //... -c opt --test_output=all
```

</details>

## Usage

```sh
lsdynatoapdl --input model.k
```

The APDL deck is written next to the input, with the `_converted.txt` suffix.

| Flag | Default | Meaning |
|:-----|:--------|:--------|
| `--input` | — | LS-DYNA deck to convert (required) |
| `--thickness_precision` | `6` | decimal places kept when grouping shell thicknesses |
| `--material` | *(none)* | `Name,E,nu,rho`, repeated with `;` |
| `--title` | from the input | analysis title for `/TITLE` and the banner |
| `--jobname` | from the input | job name for `/FILNAM` |
| `--units` | `SI` | unit system for `/UNIT` |
| `--verbose` | `false` | log every parsed record |
| `--log` | `lsdynatoapdl.log` | log file to write |

### About `--thickness_precision`

SHELL181 takes its thickness from a section, so one `SECTYPE`/`SECDATA` pair is
emitted per distinct thickness. Real decks often carry a slightly different
thickness on every element, which at full precision produces one section per
element — valid APDL, but unusable in practice. Lowering the precision merges
them:

```
--thickness_precision=6   11241 sections   (one per element)
--thickness_precision=2     162 sections
--thickness_precision=1      20 sections
```

The converter warns when the sections it produced approach the element count.

## Output format

The deck follows the layout of a hand-written Ansys model: a `/COM` banner, the
session setup, the scalar parameters, and only then the model inside `/PREP7`.

```apdl
/COM,  ---------------------------------------------------------------------------------
/COM,   PROBLEM: Helicopter tail boom modal analysis shell model
/COM,  ---------------------------------------------------------------------------------
/COM,   Converted from tail.k by LsDynaToAPDL
/COM,   nodes: 11683, keypoints: 0, shells: 11437, sections: 162
/COM,  ---------------------------------------------------------------------------------

FINISH
/CLEAR, START, NEW
/FILNAM, Shellmodel
/TITLE, Helicopter tail boom modal analysis shell model
/UNIT, SI
/INQUIRE, StrJobname, JOBNAME

! >>>>> MODEL PARAMETERS <<<<<
*SET, Pi, ACOS(-1)		!Pi constant
*AFUN, DEG			!Specify units for angular measures [DEG], specify after function *AFUN
*SET, eps, 10e-3		!precision interval

! >>>>> MATERIAL PROPERTIES <<<<<
!*** Dural
*SET, DuralEyounG, 72e9	 ![Pa] Young's modulus
*SET, DuralNi, 0.33		 !Poisson's ratio
*SET, DuralDensity, 2810	 ![Kg/m^3]

/PREP7

! >>>>> element type definition <<<<<
ET, 1, SHELL181

! >>>>> material assignment <<<<<
MP, EX, 1, DuralEyounG
MP, NUXY, 1, DuralNi
MP, DENS, 1, DuralDensity

! >>>>> section definition <<<<<
SECTYPE, 1, SHELL, , shell_1
SECDATA, 3.98, 1, 0, 3

! >>>>> node definition <<<<<
N, 1162886, -25.5204048, 320.880554, -161.429962
! >>>>> end node definition <<<<<

! >>>>> shell elements definition <<<<<
TYPE, 1
MAT, 1
SECNUM, 1
EN, 1229174, 1189339, 1162886, 1189341, 1189340
! >>>>> end elements definition <<<<<

FINISH
```

`*SET` parameters are session-level, so they precede `/PREP7`; everything that
defines the model sits inside it. `TYPE`, `MAT` and `SECNUM` are modal in APDL —
they apply to everything defined afterwards — so elements are written grouped by
section and `SECNUM` is re-emitted only when the section changes.

Coordinates keep the shortest representation that round-trips to the same
`double`, so no precision is lost relative to the source deck.

### Materials

The converter cannot know the material of a mesh, so it is given on the command
line and written as APDL parameters that the `MP` commands then refer to:

```sh
lsdynatoapdl --input tail.k \
  --material "Dural,72e9,0.33,2810;Aluminium,64e9,0.34,2700" \
  --title "Helicopter tail boom modal analysis shell model" \
  --jobname Shellmodel
```

Each entry is `Name,YoungsModulus,PoissonRatio,Density`, repeated with `;`.
Values are validated as numbers but written out exactly as typed, so `72e9`
stays `72e9` rather than becoming `72000000000`. Without `--material` the deck
is still produced, but it declares none and says so in the file — Ansys would
otherwise stop at the first section referring to material 1.

## Library and C API

The converter is also a library. From C++:

```cpp
#include "apdl/deck_writer.hh"
#include "lsdyna/converter.hh"

lsdynatoapdl::lsdyna::ConverterSyntax converter;
converter.set_input_file("model.k");
if (converter.parse()) {
  lsdynatoapdl::apdl::DeckWriter writer;
  writer.set_filename("model.inp");
  writer.write(converter.model());
}
```

From C, or any language with a C FFI:

```c
#include "lsdynatoapdl/lsdynatoapdl.h"

lsdynatoapdl_status status =
    lsdynatoapdl_convert_file("model.k", "model.inp", 2);
if (status != LSDYNATOAPDL_OK) {
  fprintf(stderr, "%s\n", lsdynatoapdl_status_message(status));
}
```

The C API uses opaque handles, reports failures as status codes, and never lets
an exception cross the boundary. A model can also be built element by element
with `lsdynatoapdl_model_add_node`, `_add_keypoint` and `_add_shell181`.

### Consuming an installed build

```cmake
find_package(LsDynaToApdl REQUIRED)
target_link_libraries(app PRIVATE LsDynaToApdl::LsDynaToApdl)  # or ::apdl, ::capi, ...
```

```sh
cc app.c $(pkg-config --cflags --libs lsdynatoapdl)
```

Install with a real package manager behind it — the vcpkg presets, or system
packages. A build that downloaded its dependencies through `FetchContent` runs
and tests fine, but CMake does not install those dependencies, so the prefix
would not be self-contained; configure prints a note when that applies.

## Architecture

```
core   command line entry point
  |
lsdyna reads the deck, one Parser per keyword section
  |
apdl   entities and the deck writer
  |
common file handling
```

APDL entities use **CRTP** rather than virtual inheritance: each type derives
from `ApdlEntity<Derived>` and supplies a single `apdl_command()`. The base
then provides `to_apdl()`, `operator<<` and an fmt formatter for every entity,
resolved at compile time. Entities stay trivially copyable with no vtable
pointer, so a deck with millions of nodes packs contiguously into a `vector`.

```cpp
class Node : public ApdlEntity<Node> {
 public:
  [[nodiscard]] auto apdl_command() const -> std::string;
};
```

That one function makes the entity printable through all three routes, so
callers use whichever their code already speaks:

```cpp
std::cout << node;                       // ostream
fmt::print("{}\n", node);                // fmt
std::string line = std::format("{}", node);  // std::format, where available
```

Format specifiers apply to the rendered command (`fmt::format("{:>12}", kp)`),
because the formatters derive from the `std::string` formatter.

The LS-DYNA side uses ordinary runtime polymorphism instead, because the active
parser is chosen from a keyword read at runtime.

### Older Apple toolchains

Floating-point `std::from_chars` was the last part of `<charconv>` to land.
libc++ guards it with an availability attribute, so on macOS Ventura with
Apple Clang 15 the declaration exists but calling it fails to compile:

```
error: 'from_chars' is unavailable: introduced in macOS 26.0
```

CMake probes for it by compiling the call with the real deployment target and
selects a `strtod_l` parser bound to the C locale when it is missing — the
locale matters, since plain `strtod` reads `1.5` as `1` wherever the decimal
separator is a comma. Both parsers accept exactly the same inputs and produce
byte-identical decks; `LSDYNATOAPDL_FORCE_FROM_CHARS_FALLBACK=ON` exercises the
fallback anywhere, and CI builds it on every run.

## Testing

```sh
ctest --test-dir build --output-on-failure
```

Documentation:

```sh
cmake -S . -B build -DBUILD_DOCUMENTATION=ON
cmake --build build --target doc_doxygen
```

## Motivation

This project was born in 2015, during the development of the thesis for a
bachelor degree in mechanical engineering, to convert LS-DYNA output files into
APDL files readable by Ansys and Workbench.

## Contributing

Knowledge of LS-DYNA and APDL syntax is the useful part. Before committing:

```sh
pre-commit install --install-hooks
pre-commit install --hook-type commit-msg
```

Commits follow [Conventional Commits](https://www.conventionalcommits.org);
`cz commit` writes a conforming message.

Style is enforced in two layers: `clang-format` for layout, and `clang-tidy`
for everything layout cannot see — naming, modernisation and bug patterns. The
tree is clang-tidy clean, so keep it that way:

```sh
cmake --preset x64-linux-debug -DLSDYNATOAPDL_ENABLE_CLANG_TIDY=ON
cmake --build out/build/x64-linux-debug
```

## Authors

- **Francesco Argentieri** — francesco.argentieri89@gmail.com

## License

This project is licensed under the GPL v3 License - see the
[LICENSE](https://github.com/frank1789/LsDynaToAPDL/blob/master/LICENSE) file
for details.
