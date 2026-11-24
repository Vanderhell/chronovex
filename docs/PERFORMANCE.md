# Performance and memory evidence

## Historical ESP32-S3 measurement

The following measurements come from a targeted earlier build of the four Chronovex algorithms. The 2026-11-24 namespace/CMake update renamed API symbols and source paths without changing the arithmetic bodies, but this revision has not been rebuilt and benchmarked on an ESP32-S3. Treat every value below as historical, not a new current-revision result.

| Property | Recorded value |
|---|---|
| Target / clock | ESP32-S3, CPU 240 MHz, 40 MHz crystal |
| Toolchain | ESP-IDF 5.5.1, Espressif GNU 14.2.0 |
| Optimization | Performance optimization enabled (`-O2`); library component strict C99 |
| Duration scale input | `1234567 * 3 / 2` |
| Modular elapsed input | `start=0xfffffff0`, `now=0x2f` |
| Deadline input | `now=0x123456789`, `delay=0x13579bdf` |
| Remaining-time input | `now=123456`, `deadline=123999` |
| Sampling | 1,000 calls per batch, 31 paired samples, alternating operation order; median actual-minus-no-op batch cost divided by calls per batch |
| Separately measured empty-loop overhead | 15 cycles/call; paired no-op baseline was also subtracted |
| `chronovex_anim_duration_scale_u32` | 206 net cycles/call |
| `chronovex_motion_duration_u32` | 0 whole net cycles/call at the measurement's integer resolution; this is below one-cycle-per-call resolution, not zero physical execution cost |
| `chronovex_time_deadline_from_now_u64` | 14 net cycles/call |
| `chronovex_time_until_deadline_i64` | 13 net cycles/call |
| Isolated library flash | 270 bytes code in the library component report |
| Isolated static RAM | 0 bytes BSS/data in the library component report |
| Complete test app | 191,488 bytes; includes framework and harness and is not library-only size |
| Stack observation | Task configured for 3,584 bytes; one exercised path left 1,964 bytes free, or 1,620 bytes observed use. This is not a maximum stack bound. |

Per-call figures are workload-, compiler-, target- and methodology-specific. They are observed medians, not worst-case execution guarantees. The stack watermark describes only the exercised run.

## Current revision

Current host builds validate the renamed API, install target, and examples. Current ESP32-S3 cycle counts, isolated code size, static RAM and stack usage are **NOT MEASURED**. The previous targeted firmware reported all four APIs passing representative checks; current source names were not flashed. See [Testing](TESTING.md) for the hardware-evidence distinction.

## Source-level notes

The four production translation units contain no file-scope mutable data and no dynamic allocation. This is an implementation inspection, not a target footprint measurement.
