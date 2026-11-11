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

## Remote GitHub Actions

Workflow [43005668032](https://github.com/Vanderhell/chronovex/actions/runs/43005668032) on commit `32b2e30bee2a6267aaa77bdcbae9386a970d4dc2` completed **8/8 PASS**: GCC Debug/Release, Clang Debug/Release, MSVC Debug/Release, GCC ASan, and GCC UBSan. This run supplies the sanitizer validation unavailable in the local Windows MinGW environment.

## ESP32-S3

**PASS: COM37, ESP32-S3.** Following the user's explicit instruction to run this project on COM37, a dedicated ESP-IDF validation image was built using ESP-IDF 5.5.1 and the Espressif GNU 14.2.0 toolchain. The test exercised all four APIs against independent expected boundary values and emitted `CHRONOVEX_ALL4_PASS|passed=4/4|first_failed_api=0`.

Only app0 (`0x10000`, `0x140000` bytes) was temporarily replaced. Before writing, the complete app0 image was backed up (1,310,720 bytes; SHA-256 `553B9D53A6316A3FD7F08AE9F65003E055CD6C6CE94FEF28B862EACF72464D4A`). After the test, the original image was restored and all 20 readback chunks were checked; the restored size and SHA-256 matched exactly. NVS, OTA data, bootloader, partition table and other partitions were not written. The device was reset to run the test and after restoration.

The hardware vectors include nearest-even duration ties and invalid scaling inputs, timer wrap/zero/maximum elapsed values, checked deadline addition at `UINT64_MAX` and overflow, and signed deadline differences at `INT64_MIN`/`INT64_MAX` and unrepresentable boundaries. This is targeted device validation, not exhaustive testing; the host differential suite remains the broad numerical check. Hardware benchmark configuration and results are recorded in [PERFORMANCE.md](PERFORMANCE.md).

## Remaining limits

- The device test covered targeted vectors rather than the host suite's 100,000 cases per API.
- The observed stack watermark is for this validation firmware's exercised path; it is not a proof of maximum stack use under all states or future builds.
- Randomized tests cannot prove all 64-bit input combinations.
- No confirmed production arithmetic defect was found by the implementation review; this is not a formal proof.
