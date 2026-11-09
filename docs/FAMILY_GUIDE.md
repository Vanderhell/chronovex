# Chronovex family guide

## What this family solves

**Timing and scheduling.** Wrap-safe deadlines, periods, durations, timeouts and schedule calculations.

This guide describes the complete public family as an independent embedded-C product. Each primitive remains separately linkable; select only the headers and source units required by the application.

## When to use this family

Use Chronovex when the application needs the operations listed below and requires deterministic behavior, explicit caller-owned memory, and no mandatory operating-system services.

## Family at a glance

```mermaid
flowchart LR
    A[Application input] --> B[Validate representation and bounds]
    B --> C[Select one public primitive]
    C --> D[Check status or output contract]
    D --> E[Use result with documented units]
```

## Primitive map

| Public header | Symbol | Purpose and contract | Validation source |
|---|---|---|---|
| `include/chronovex/timing/pc_anim_duration_scale_u32.h` | `pc_anim_duration_scale_u32` | Return nearest-even(duration * scale_num / scale_den). Failure preserves *result for zero denominator, uint32 result overflow, or null result. | tests/test_implementation.c |
| `include/chronovex/timing/pc_motion_duration_u32.h` | `pc_motion_duration_u32` | Elapsed modulo-2^32 timer ticks from start to now. The actual interval must be less than one complete timer cycle; multiple wraps are ambiguous. | tests/driver.c |
| `include/chronovex/timing/pc_time_deadline_from_now_u64.h` | `pc_time_deadline_from_now_u64` | Checked non-wrapping deadline = now + delay. False on null output or uint64 overflow; failure leaves *deadline unchanged. | No enabled test source was found by symbol search. |
| `include/chronovex/timing/pc_time_until_deadline_i64.h` | `pc_time_until_deadline_i64` | Store signed deadline-now for uint64 timestamps. Positive means future, negative means overdue. False if result is outside int64 or output is null; failure leaves *remaining unchanged. | tests/test_basic.c |

## Selection guidance

- Choose the primitive whose width, representation, rounding, saturation, wrapping, and error contract match the application.
- Do not substitute a similarly named function without checking the public header.
- Allocate state and buffers in the caller; respect documented aliasing and output-preservation rules.
- Treat entries marked without a validation source as requiring additional product-level tests before release.

## Integration checklist

1. Include only the required public header.
2. Compile the matching source translation unit or use the product CMake target.
3. Check every boolean/status return before consuming an output.
4. Add application-specific boundary tests for units, widths, invalid inputs, and overflow behavior.
5. Run the documented host tests before target integration.

## Complexity and footprint

The product is split into independent translation units. Exact flash, RAM, stack, and cycle costs depend on the selected functions, compiler, optimization, and MCU; unmeasured values remain explicitly marked PENDING in the testing documentation.

## Validation boundary

The API reference links each public entry to its implementation and available validation source. Hardware status is reported only when reproducible target evidence exists.
