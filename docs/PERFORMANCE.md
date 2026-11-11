# Performance

## ESP32-S3

**Measured on COM37, ESP32-S3**, at the user's explicit direction. The test image was temporary; app0 was restored and its full readback SHA-256 matched the pre-test backup. No other partition was written.

| Metric | Status |
|---|---|
| SoC / clock | ESP32-S3, CPU 240 MHz; 40 MHz crystal |
| Framework / compiler | ESP-IDF 5.5.1, Espressif GNU 14.2.0 |
| Optimization | `CONFIG_COMPILER_OPTIMIZATION_PERF=y` (`-O2`); library component compiled with strict `-std=c99`; harness uses the ESP-IDF GNU C dialect |
| Inputs | scale `1234567*3/2`; motion `start=0xfffffff0, now=0x2f`; deadline `now=0x123456789, delay=0x13579bdf`; until `now=123456, deadline=123999` |
| Repetitions / sampling | 1,000 calls per batch, 31 paired samples, alternating order; reported median of actual-minus-no-op batch costs, divided by calls per batch |
| Empty benchmark-loop overhead | 15 cycles/call, measured separately; paired no-op baselines are also subtracted for each API |
| `pc_anim_duration_scale_u32` | 206 net cycles/call |
| `pc_motion_duration_u32` | 0 whole net cycles/call at the reported integer resolution; this means the paired median rounded/truncated below one cycle per call, not that the function takes zero physical cycles |
| `pc_time_deadline_from_now_u64` | 14 net cycles/call |
| `pc_time_until_deadline_i64` | 13 net cycles/call |
| Isolated library flash | 270 bytes Flash Code in `libchronovex.a` (`idf.py size-components`); harness/runtime excluded from this component figure |
| Isolated library static RAM | 0 bytes BSS/data for the library component |
| Validation image | 191,488 bytes (`0x2ec00`) total app binary; includes ESP-IDF runtime and test harness and is not the isolated library size |
| Stack watermark | Main task configured for 3,584 bytes; observed minimum free 1,964 bytes (word size 1 byte), corresponding to 1,620 bytes observed use in this run |

Cycles are specific to these inputs, toolchain, clock and paired benchmark. The 0-cycle integer result for the very small modular subtraction is below the method's per-call integer resolution. The stack watermark describes this execution only and is not a general maximum stack bound. The app binary includes ESP-IDF and harness code; only the component report is used for isolated library flash/RAM.

## Source-level notes

The four production translation units contain no file-scope data and no dynamic allocation. The measured component report shows 270 bytes of flash code and 0 bytes of static data for this build; this does not establish costs for other toolchains/configurations or a general target stack bound.
