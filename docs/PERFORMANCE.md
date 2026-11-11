# Performance

## ESP32-S3

**HW NOT VERIFIED.** No clearly authorized Chronovex test device was identified. COM36 runs another project (Estimora), COM19 is used by Fortavis, and COM37 is reserved. No firmware was flashed for this audit.

| Metric | Status |
|---|---|
| Clock and compiler configuration | NOT MEASURED |
| Per-API cycles | NOT MEASURED |
| Isolated library flash | NOT MEASURED |
| Isolated library static RAM | NOT MEASURED |
| Stack usage | NOT MEASURED |

Do not infer Chronovex costs from host timings, other projects' firmware, or whole-sketch sizes. Future measurements should record device identity/authorization, clock, compiler and optimization flags, representative input, repeated sample count, timer and loop overhead, and isolate library contribution from runtime and harness. A single stack watermark is not a maximum stack bound.

## Source-level notes

The four production translation units contain no file-scope data and no dynamic allocation. This does not establish target stack bounds or flash cost.
