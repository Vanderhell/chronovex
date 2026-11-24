# Chronovex API reference

The primary public API consists of four namespaced functions declared under `include/chronovex/timing/`. For every API, `tests/test_audit.c` provides direct enabled boundary and deterministic differential tests. The current working tree also checks the old v0.1 source-compatible headers. Historical hardware evidence and current host evidence are distinguished in [TESTING.md](TESTING.md) and [PERFORMANCE.md](PERFORMANCE.md).

## `chronovex_anim_duration_scale_u32`

- **Header/source:** `anim_duration_scale_u32.h` / `src/anim_duration_scale_u32.c`
- **Purpose:** Scale an unsigned duration by `scale_num / scale_den`.
- **Inputs:** Any `uint32_t duration`, `scale_num`; `scale_den` must be nonzero.
- **Output:** `round_nearest_even(duration * scale_num / scale_den)` when the rounded result fits `uint32_t`.
- **Failure:** Returns `false` for a null result pointer, zero denominator, or result above `UINT32_MAX`. The output is unchanged.
- **Arithmetic:** `duration * scale_num` is exact in `uint64_t`: `(2^32 - 1)^2 < 2^64`. No intermediate multiplication overflow occurs. Half ties round to an even integer, including ties that round down to zero.
- **Coverage:** Fixed ties, zero inputs/denominator, maximum inputs, result overflow, output preservation, plus 100,000 deterministic exact-reference cases.

## `chronovex_motion_duration_u32`

- **Header/source:** `motion_duration_u32.h` / `src/motion_duration_u32.c`
- **Purpose:** Return `now - start` modulo `2^32` for a wrapping hardware counter.
- **Inputs/output:** Any pair of `uint32_t`; always returns a `uint32_t`.
- **Failure:** None; there are no pointers or invalid bit patterns.
- **Wrap contract:** Correctly represents elapsed ticks only if the actual interval is in `[0, 2^32 - 1]` ticks. A zero result could mean no elapsed ticks or one/more full counter cycles. The function has no state and cannot recover multiple wraps. Do not use this result as a generic absolute-deadline ordering comparison.
- **Coverage:** Zero, boundary crossing over `UINT32_MAX`, maximum returned interval, plus 100,000 deterministic cases against an independent modulo reference.

## `chronovex_time_deadline_from_now_u64`

- **Header/source:** `time_deadline_from_now_u64.h` / `src/time_deadline_from_now_u64.c`
- **Purpose:** Construct an absolute, non-wrapping timestamp `now + delay`.
- **Inputs:** Any `uint64_t now` and `delay`.
- **Output:** Exact sum if it is at most `UINT64_MAX`.
- **Failure:** Returns `false` for a null output pointer or an unrepresentable sum; output is unchanged. Zero delay succeeds even when `now == UINT64_MAX`.
- **Coverage:** Zero, exact maximum, overflow, null output, output preservation, and 100,000 deterministic cases checked using independent 32-bit-limb addition.

## `chronovex_time_until_deadline_i64`

- **Header/source:** `time_until_deadline_i64.h` / `src/time_until_deadline_i64.c`
- **Purpose:** Return signed absolute difference `deadline - now` for `uint64_t` timestamps.
- **Inputs:** Any `uint64_t now` and `deadline`.
- **Output:** Positive if future, zero if equal, negative if overdue, provided the exact difference lies in `[INT64_MIN, INT64_MAX]`. `INT64_MIN` is representable for overdue magnitude `2^63`.
- **Failure:** Returns `false` when the signed difference is unrepresentable or output is null. Output is unchanged.
- **Representation limit:** This signed result cannot describe positive remaining time greater than `INT64_MAX`, or overdue magnitude greater than `2^63`.
- **Coverage:** Positive/zero/negative cases, both signed result limits, unrepresentable differences, timestamps near `UINT64_MAX`, null/output preservation, and 100,000 deterministic cases against an independent two-limb difference reference.

## Integration

Include `<chronovex/timing/<header>.h>` and compile the corresponding source unit, or link the CMake target `Chronovex::Chronovex`. Check boolean results before consuming outputs. No dynamic allocation or external runtime service is required by these functions.
