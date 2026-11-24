# Testing and validation

## Direct API coverage

The enabled host suite calls all four public `chronovex_` APIs directly. `tests/test_audit.c` runs 100,000 deterministic differential cases per API and targeted limits, invalid inputs, wrap behavior, rounding boundaries, and output-preservation checks. The independent references use limb arithmetic for multiplication, addition and subtraction; modular elapsed time uses a separate modulo reference. Checks are explicit and remain active in Release.

The strict CTest configuration includes ten entries when tests, examples and the optional C++ smoke test are enabled: four C API programs, four separately compiled public-header smoke tests, the C++98 header/link check (when a C++ compiler is available), and the quick-start example. The installed-package consumer is also built and run separately by CI.

## Reproduce

```sh
cmake -S . -B build -DCHRONOVEX_BUILD_TESTS=ON -DCHRONOVEX_BUILD_EXAMPLES=ON -DCHRONOVEX_BUILD_CXX_HEADER_TEST=ON -DCHRONOVEX_STRICT_WARNINGS=ON -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
ctest --test-dir build --output-on-failure
```

Repeat with `-DCMAKE_BUILD_TYPE=Release` in a separate build directory. GCC and Clang use `-Wall -Wextra -Wpedantic -Werror`; MSVC-compatible builds use `/W4 /WX`. CTest checks are not based on `assert()`.

## Local validation of the namespaced API migration

Executed 2026-11-24 against the current working tree:

| Toolchain / configuration | Result |
|---|---|
| GCC 16.1.0 / Debug | PASS, 10/10 CTest |
| GCC 16.1.0 / Release | PASS, 10/10 CTest |
| MSVC 19.42.34444 / Debug | PASS, 10/10 CTest |
| MSVC 19.42.34444 / Release | PASS, 10/10 CTest |
| Visual Studio ClangCL 19.1.5 / Debug | PASS, 10/10 CTest |
| Visual Studio ClangCL 19.1.5 / Release | PASS, 10/10 CTest |
| Windows GNU-mode Clang 22.1.8 | NOT RUN: the installed linker environment lacks `oldnames.lib` and `msvcrtd.lib`; ClangCL was used and passed. |
| GCC AddressSanitizer | NOT RUN locally: MinGW linker cannot find `-lasan`; the WSL environment was inaccessible. |
| GCC UndefinedBehaviorSanitizer | NOT RUN locally: MinGW linker cannot find `-lubsan`; the WSL environment was inaccessible. |

Additional executed checks:

- Fresh Release install to a local prefix and external `find_package(Chronovex 0.2 CONFIG REQUIRED)` consumer using GCC 16.1.0: PASS (1/1 CTest).

- Installed Release package consumed through external `find_package(Chronovex CONFIG REQUIRED)`: PASS.
- External `add_subdirectory()` C consumer linked `Chronovex::Chronovex`: PASS.
- The previous v0.1 include paths and function spellings compile and run through compatibility headers: PASS.
- Public headers compile and link from C++98 using ClangCL and MSVC, and from GNU C++98: PASS.

The namespaced changes have passed the six local host configurations above. Remote validation of commit `d7fe9a80a0b5f1847cb56053509ad7dc67da0647` completed successfully: all 9 GitHub Actions jobs passed, including GCC/Clang Debug and Release, MSVC Debug and Release, ASan, UBSan, and installed-package consumption. Workflow: https://github.com/Vanderhell/chronovex/actions/runs/45462832030.

## ESP32-S3

A historical targeted device run exercised all four APIs and reported `CHRONOVEX_ALL4_PASS|passed=4/4|first_failed_api=0` at 240 MHz using ESP-IDF 5.5.1 and Espressif GNU 14.2.0. It used representative and boundary vectors, not the 100,000-case host differential suite. The current namespaced source revision has not been flashed or rerun on hardware; treat the result as historical evidence only.

## Limits

- The 64-bit input domains are not exhaustively enumerated; seeded differential tests do not prove all possible values.
- The targeted historical device test does not establish full-domain behavior, a universal stack bound, or performance for other toolchains/configurations.
- Historical package/module tests that are not in the enabled target list remain preserved in the repository and are not counted as current passing CTest tests.
