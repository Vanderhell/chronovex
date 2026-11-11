# Testing and validation

## Inventory and coverage

- Four public callable APIs are declared by four headers under `include/chronovex/timing/`.
- `tests/test_audit.c` directly calls all 4/4 APIs. Each receives 100,000 deterministic fixed-seed differential cases, alongside targeted boundary and invalid-input checks.
- Scaling uses a base-2^16 limb multiplication plus binary division oracle, independent of the production uint64 product/division sequence.
- Deadline construction uses a two-limb addition oracle. Signed deadline difference uses two-limb subtraction and a distinct negative conversion. Modular elapsed time uses an explicit modulo reference.
- Tests use explicit checks, not `assert()`, so Debug and Release both execute every condition.
- Boundary checks include nearest-even ties, zero denominator, maximum uint32 factors, product/result limits, counter wrap and zero elapsed, full-width absolute timestamp addition, `INT64_MIN`/`INT64_MAX` differences, null pointers, and output preservation.

The 64-bit random domains are not exhaustively enumerated; deterministic differential testing is evidence, not a proof for all possible values.

## Reproduce

```sh
cmake -S . -B build -DCHRONOVEX_BUILD_TESTS=ON -DCHRONOVEX_STRICT_WARNINGS=ON
cmake --build build --config Release --parallel
ctest --test-dir build -C Release --output-on-failure
```

CMake selects C99 with extensions disabled. GCC and Clang use `-Wall -Wextra -Wpedantic -Werror`; MSVC uses `/W4 /WX`. Tests remain enabled only when `CHRONOVEX_BUILD_TESTS=ON`.

## Local host results (2026-11-11)

| Compiler | Configuration | Result |
|---|---|---|
| GCC 16.1.0 (MinGW UCRT) | Debug | PASS, 3/3 CTest |
| GCC 16.1.0 (MinGW UCRT) | Release | PASS, 3/3 CTest |
| Clang 19.1.5 (Visual Studio ClangCL toolset) | Debug | PASS, 3/3 CTest |
| Clang 19.1.5 (Visual Studio ClangCL toolset) | Release | PASS, 3/3 CTest |
| MSVC 19.42.34444 | Debug | PASS, 3/3 CTest |
| MSVC 19.42.34444 | Release | PASS, 3/3 CTest |
| GCC ASan (local Windows/MinGW) | Debug | NOT RUN: `libasan` runtime is not installed in this toolchain |
| GCC UBSan (local Windows/MinGW) | Debug | NOT RUN: `libubsan` runtime is not installed in this toolchain |

Strict C99/warnings-as-errors passed in the compiler builds above. The direct Clang Windows GNU link attempt was unavailable because matching Windows GNU runtime libraries are not installed; the Visual Studio ClangCL toolset was used and passed both configurations.

## ESP32-S3

**HW NOT VERIFIED.** Previously identified COM36 belongs to Estimora, COM19 to Fortavis, and COM37 is reserved. No device is clearly authorized for Chronovex firmware, so no Chronovex image was flashed and no device was reset or modified for this project.

## Remaining limits

- Hardware output, cycles, flash, static RAM, and stack have not been measured for Chronovex.
- Randomized tests cannot prove all 64-bit input combinations.
- No confirmed production arithmetic defect was found by the implementation review; this is not a formal proof.
