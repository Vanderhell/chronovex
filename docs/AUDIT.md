# Chronovex technical audit register

Audit date: 2026-11-11. Scope: all four public headers and implementation units, enabled and preserved test sources, CMake, CI, and product documentation. No production algorithm or public signature was changed.

## Public API inventory

| API | Implementation | Direct enabled test |
|---|---|---|
| `pc_anim_duration_scale_u32` | `src/pc_anim_duration_scale_u32.c` | `tests/test_audit.c` |
| `pc_motion_duration_u32` | `src/pc_motion_duration_u32.c` | `tests/test_audit.c` |
| `pc_time_deadline_from_now_u64` | `src/pc_time_deadline_from_now_u64.c` | `tests/test_audit.c` |
| `pc_time_until_deadline_i64` | `src/pc_time_until_deadline_i64.c` | `tests/test_audit.c` |

## Findings

| ID | Classification | Finding | Disposition/evidence |
|---|---|---|---|
| C-01 | BUG (test coverage) | `pc_time_deadline_from_now_u64` was not directly covered by an enabled test; the test CMake list enabled only two programs. | Added `test_audit.c`; direct checks and 100,000 deterministic differential cases for deadline addition. |
| C-02 | BUG (Release validation) | Existing enabled tests used `assert()`, which disappears under `NDEBUG`. | Replaced assertions with explicit failure checks; Release GCC/Clang/MSVC runs execute all checks. |
| C-03 | BUG (documentation accuracy) | README described the family broadly as wrap-safe deadlines; API docs had a missing test placeholder. | Rewrote timing-contract summaries and API evidence. Only 32-bit elapsed subtraction is modular; deadline construction is checked absolute addition. |
| C-04 | OK (production arithmetic) | Duration scaling product, rounding, checked deadline addition, signed difference, and modulo elapsed subtraction reviewed. | No reproducible production defect found. See details below and direct differential test evidence. |

## Algorithm review

- **Scaling:** all `uint32_t × uint32_t` products fit in `uint64_t`; nearest-even compares remainder with denominator minus remainder, avoiding doubled-remainder overflow. A rounded quotient above `UINT32_MAX` fails without writing output.
- **Modular elapsed:** unsigned subtraction is defined modulo `2^32`. Actual elapsed is unambiguous only in `[0, 2^32-1]` ticks; full-cycle/multiple-cycle history is not recoverable. It is not an absolute deadline comparison.
- **Deadline construction:** checks representability of exact `now + delay` before addition; overflow and null output fail without modifying the output.
- **Signed remaining time:** ordered unsigned subtraction avoids underflow; representable interval is `[-2^63, 2^63-1]`. The `-2^63` boundary is handled without negating `INT64_MIN`.
- All operations are deterministic and use fixed-width integer arithmetic. No mandatory heap/OS dependency or file-scope state is present in the production C units.

## Validation evidence

All four APIs have direct enabled tests. The test suite ran 100,000 fixed-seed differential cases per API and explicit boundary/failure checks. GCC 16.1.0, Visual Studio ClangCL 19.1.5, and MSVC 19.42.34444 passed Debug and Release, 3/3 CTest each. Strict warnings are errors. Local Windows MinGW lacks `libasan` and `libubsan`; sanitizer results will be reported from the remote workflow when complete.

Hardware is **HW NOT VERIFIED**. COM19 and COM36 belong to unrelated projects; COM37 is reserved. No Chronovex firmware was built or flashed. Cycles, isolated flash/RAM, and stack remain unmeasured.

## Remaining limitations

- Randomized testing is not exhaustive over the 64-bit domains.
- No hardware evidence is available for timing or memory usage.
- No confirmed production arithmetic defect was reproduced; tests and review do not constitute a formal proof.
