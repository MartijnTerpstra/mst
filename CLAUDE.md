# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project

`mst` is a header-mostly C++17 utility library (containers, multithreading primitives, math types/algorithms,
platform/compiler/CPU detection, misc utilities like uuid/compiletime/hashing). It's a hobby project by
Martijn/Martinus Terpstra. See `README.md` for the feature list.

## Build & test

Requires CMake >= 3.13 and a C++17 compiler (MSVC, Clang, or GCC). On non-ARM targets the test build adds `-mavx`
(GCC/Clang) — a native/AVX-capable CPU is assumed for running tests.

Builds are driven by `CMakePresets.json` at the repo root. Local dev presets are `dev-debug`/`dev-release` (both
turn on `MST_RUN_TESTS` and `MST_UTILS`, output in `./build`); CI uses its own presets (`ci-tests`, `ci-coverage`,
`ci-licenses`) invoked directly from the workflow YAML in `.github/workflows/`. Using presets requires
CMake >= 3.21 (only for the presets themselves — the project still builds with plain `-D` flags on older CMake).

The presets file also carries over what used to be Visual Studio's `CMakeSettings.json` (now deleted — VS reads
`CMakePresets.json` natively): `windows-strict-*` (stricter `/W4` warnings, Windows-only), and `linux-gcc-*` /
`linux-clang-*` (explicit-compiler Linux builds, each in their own `build-linux-*` dir so they don't clobber each
other). Both inherit `dev-base`; the `linux-gcc-*`/`linux-clang-*` pair also inherit the hidden `linux-base`,
which carries the `hostSystemName == Linux` condition plus a `vendor` block excluding local build dirs from the
rsync-based source copy Visual Studio does when the active CMake target is remote/WSL (Tools > Options > Cross
Platform > Connection Manager, or a WSL target) — that `condition` matches both a native Linux host and a VS
remote/WSL target, since VS evaluates `hostSystemName` against the target system in that case.

```bash
./scripts/build.sh          # configure+build via the `dev-debug` preset, output in ./build
./scripts/build.sh Release   # build a specific config (maps to the `dev-release` preset)
REBUILD=1 ./scripts/build.sh # wipe ./build first (equivalent: ./scripts/rebuild.sh)
TESTING=1 ./scripts/build.sh # also run `ctest` after building        (equivalent: ./scripts/test.sh)
```

PowerShell equivalents (`scripts/build.ps1`, `scripts/rebuild.ps1`, `scripts/test.ps1`) work the same way, e.g.
`./scripts/build.ps1 Release`, `$env:REBUILD=1; ./scripts/build.ps1`.

These are just thin wrappers around plain CMake preset invocations, e.g.:

```bash
cmake --preset dev-debug
cmake --build --preset dev-debug
ctest --preset dev-debug
```

- `MST_RUN_TESTS` — fetches Catch2 (v2.13.8, via `FetchContent`) and registers one test executable per test file
  (see `add_mst_test(...)` calls at the bottom of `CMakeLists.txt`).
- `MST_UTILS` — builds the `utils/` helper executables (currently `util_update_licenses`).
- The `mst` library target itself has no build option; it's always built.

Run a single test executable (each source file under `tests/<category>/<name>.cpp` becomes
`test_<category>_<name>`):

```bash
ctest --test-dir build -C Debug -R test_common_scope_guard --output-on-failure
# or run the Catch2 binary directly for Catch2-specific filtering (tags, sections, etc.):
./build/test_common_scope_guard "[common]"
```

Tests tagged `[not_deterministic]` are excluded from CI coverage runs (`MST_TEST_ARGS`, set to
`~[not_deterministic]` by the `ci-coverage` preset) — flaky/timing-sensitive tests should carry that tag.

CI (`.github/workflows/`) builds MSVC on Windows and Clang/GCC on Linux/macOS/arm, runs `ctest`, checks license
headers, and reports coverage — mirror that matrix mentally when touching platform-conditional code.

## Code style

Formatting is enforced by `.clang-format` (LLVM-based, tabs, 100-col limit, `Custom` brace wrapping with braces
on their own line for functions/classes/control statements but not namespaces). Run `clang-format` before
committing; don't hand-format to a different style.

Every source/header file starts with an MIT license banner comment (see any existing file). New files must
include it — `util_update_licenses` (see below) checks/enforces this in CI and can rewrite headers to the
current year.

## Architecture

### Public vs. detail headers

Two header prefixes distinguish public API surface from implementation detail:

- `m*.h` in `mst/` — the public API. These are what consumers of the library `#include`.
- `mx_*.h` in `mst/` — internal/detail headers `#include`d by the `m*.h` files (e.g. `mplatform.h` pulls in
  `mx_platform.h`; `mcommon.h` pulls in `mx_common.h`, `mx_hash.h`, `mx_printf2.h`). Detail types often live in
  an `mst::_Details` namespace. Don't add new public API to an `mx_*.h` file, and don't include an `mx_*.h`
  directly from test/user code — go through its `m*.h` wrapper.

`mcore.h` is the root header almost everything depends on (base typedefs, `MST_DEBUGMODE`, epsilon, etc.), and
`mcommon.h` layers general-purpose helpers (e.g. `non_copyable`, `zeromem`) on top of it.

Everything lives under the `mst` namespace (with `mst::_Details` for internals).

### Compiled parts

The library is almost entirely header-only; only four translation units are compiled into the static `mst` lib
(see `add_library(mst STATIC ...)` in `CMakeLists.txt`):
`src/mplatform.cpp`, `src/mthreading.cpp`, `src/mthreading_rw_lock.cpp`, `src/muuid.cpp`. These back the
platform-detection, threading, and uuid headers where the implementation can't be fully inline (e.g. OS calls).
On Apple/Unix, `mst` links `pthread`.

### Platform/compiler/CPU detection

`mplatform.h`/`mx_platform.h` and related headers (`mcompiletime.h`, `mflag.h`) provide OS (`MST_PLATFORM_*`),
compiler, and CPU-vendor detection used to branch behavior (e.g. `platform::newline()` differs per OS). SIMD
math (`msimd.h`, `msimdmath.h`, `mx_math_*_simd.inl`) assumes AVX is available in test builds; non-x86
(aarch64/arm64) builds skip the `-mavx` flag in `CMakeLists.txt`.

### Test layout

`tests/<category>/<name>.cpp` mirrors the conceptual grouping used in `CMakeLists.txt`'s `add_mst_test` calls
(categories: `algorithm`, `common`, `containers`, `lock_free`, `math`, `memory`, `platform`, `test_util`, `text`,
`threading`). Each file defines `CATCH_CONFIG_MAIN` and is its own standalone Catch2 executable, tagged by
category, e.g. `TEST_CASE("...", "[common]")`. Shared test
helpers (random data generation, set assertions, misc helpers) live in `tests/test_util/` and are exposed via
`tests/test_util/helpers.h` etc. — link/include from there rather than duplicating test scaffolding. New tests
must be registered with `add_mst_test(<category> <name>)` in `CMakeLists.txt` to be picked up.

### `utils/`

Standalone helper executables built only with `-DMST_UTILS=True`, registered via `add_util(...)` in
`CMakeLists.txt`. Currently `utils/update_licenses` — walks the repo checking/rewriting the MIT license header
banner on source files (used by the `Check Licenses` CI workflow with `--fail-on-missmatch`).
