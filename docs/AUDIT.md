# Chronovex audit register

Audit update: 2026-11-24. Scope: four public timing APIs, namespace migration, public headers, CMake install/export configuration, compatibility headers, enabled tests, examples, and existing hardware/performance evidence.

## API and migration inventory

| Primary API | Public header | Implementation | Direct enabled test |
|---|---|---|---|
| `chronovex_anim_duration_scale_u32` | `include/chronovex/timing/anim_duration_scale_u32.h` | `src/anim_duration_scale_u32.c` | `tests/test_audit.c` |
| `chronovex_motion_duration_u32` | `include/chronovex/timing/motion_duration_u32.h` | `src/motion_duration_u32.c` | `tests/test_audit.c` |
| `chronovex_time_deadline_from_now_u64` | `include/chronovex/timing/time_deadline_from_now_u64.h` | `src/time_deadline_from_now_u64.c` | `tests/test_audit.c` |
| `chronovex_time_until_deadline_i64` | `include/chronovex/timing/time_until_deadline_i64.h` | `src/time_until_deadline_i64.c` | `tests/test_audit.c` |

The old v0.1 header paths remain forwarding compatibility headers and map their former function spellings to the namespaced API. No duplicate old linker symbols are emitted; consumers must rebuild. The mapping is recorded in the internal migration register.

## Correctness findings

No production arithmetic was changed in this update. The algorithms retain their previously audited semantics:

- Duration scaling multiplies two `uint32_t` values exactly in `uint64_t`, rounds ties to even, rejects zero denominator and out-of-range results, and preserves output on failure.
- Wrapping elapsed time uses defined unsigned subtraction modulo `2^32`; a single returned value cannot disambiguate one or more full counter cycles.
- Absolute deadline construction rejects `now + delay` overflow and preserves the output on failure.
- Signed remaining time accepts only exact differences in `[-2^63, 2^63-1]`, including `INT64_MIN` without negating it, and preserves output on failure.

The new C++ smoke test exposed a test-only assumption: `UINT32_MAX` is not guaranteed by the selected C++98 headers. The vector now uses `~uint32_t(0)`. Initial compatibility/quick-start tests also had the `start, now` arguments reversed; those test vectors were corrected to the header contract. These test defects did not identify a production algorithm defect.

## Build and integration findings

- Replaced CMake source globbing with an explicit four-file list.
- Added the `Chronovex::Chronovex` alias/export, install rules, package config and version files.
- Tests and examples are opt-in and disabled by default for consumers.
- CTest names are library-prefixed; test checks stay enabled in Release.
- Added C++ linkage guards to the four primary headers and kept a source-compatibility path.
- GCC Debug/Release, MSVC Debug/Release and Visual Studio ClangCL Debug/Release each passed 6/6 CTest locally.
- External installed `find_package()` and `add_subdirectory()` C consumers passed locally.

## Remaining validation limits

- Local ASan and UBSan could not be linked: the available MinGW installation lacks `libasan` and `libubsan`, and WSL access was denied. The updated remote sanitizer jobs have not yet run on this source revision.
- Windows GNU-mode Clang could not link the Windows runtime libraries; Visual Studio ClangCL passed Debug and Release.
- Historical ESP32-S3 evidence is 4/4 representative checks. The namespaced source revision has not been rerun on hardware.
- Preserved non-enabled package/module tests are not counted as current passing tests. Their ownership and migration remain part of the portfolio-wide reconciliation.

## 2026-11-24 working-tree validation

After adding separate C compilation tests for each public header, GCC 16.1.0, MSVC 19.42.34444 and Visual Studio ClangCL 19.1.5 each passed Debug and Release with 10/10 CTest tests. Sanitizers and the new remote workflow remain pending; this does not supersede the historical hardware evidence described above.
