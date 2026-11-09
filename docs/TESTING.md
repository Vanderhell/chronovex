# Testing

| Test category | Count | Platform | Result | Evidence |
|---|---:|---|---|---|
| Self-contained CTest tests | 2 | MSVC host | PASS | `test_basic`, `test_implementation`. |
| Preserved test sources | 9 | Repository | PRESERVED | Non-product dependencies are not enabled. |
| Hardware tests | 0 | ESP32-S3 | NOT TESTED | HW NOT VERIFIED. |

Reproduce with `cmake -S . -B build -DCHRONOVEX_BUILD_TESTS=ON`,
`cmake --build build --config Release`, and
`ctest --test-dir build --output-on-failure -C Release`.

Known gap: cycle, stack and MCU-specific timing measurements are not available.
