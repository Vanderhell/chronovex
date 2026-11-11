# Chronovex family guide

Chronovex provides four small timing and duration operations for embedded C:

- Scale a duration with exact integer arithmetic and nearest-even rounding.
- Calculate elapsed ticks from a wrapping 32-bit counter.
- Construct a checked absolute 64-bit deadline.
- Compute a signed remaining-time difference when it fits in `int64_t`.

## Choose by time representation

| Situation | API | Important limit |
|---|---|---|
| Timer hardware counter wraps at 32 bits | `pc_motion_duration_u32` | Correct only when elapsed interval is less than `2^32` ticks; multiple cycles are ambiguous. |
| Absolute timestamp plus delay | `pc_time_deadline_from_now_u64` | Rejects sum overflow; no modular wrap behavior. |
| Signed remaining/overdue duration | `pc_time_until_deadline_i64` | Exact difference must fit in `int64_t`. |
| Integer duration scaling | `pc_anim_duration_scale_u32` | Nonzero denominator; nearest-even rounding; result must fit `uint32_t`. |

Only `pc_motion_duration_u32` supports modular wrap arithmetic. Chronovex does not provide a universal wrap-safe deadline comparison.

## Integration checklist

1. Include the header under `chronovex/timing/` and build its matching source unit, or use the CMake target.
2. Keep counter ticks and absolute timestamps as distinct concepts.
3. Check status results and use outputs only after success.
4. Test application-specific units and deadlines at their own bounds.
5. Run Debug and Release tests; checks are explicit and do not disappear with `NDEBUG`.

All APIs use caller-owned values and pointers. Production implementations do not allocate memory or depend on an operating system.
