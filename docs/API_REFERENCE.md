# Chronovex API reference

This reference explains every public header in the Chronovex product. It is generated from the current headers and test sources; the header remains authoritative when a detail conflicts.

## Reading the entries
Each entry states the public symbol, implementation unit, contract, edge/failure behavior and a call extracted from the validating test source. The examples are evidence-oriented; adapt buffer ownership and numeric units to the application.

## include/chronovex/timing/pc_anim_duration_scale_u32.h

**Public symbol:** pc_anim_duration_scale_u32

**Implementation unit:** src/pc_anim_duration_scale_u32.c

**Purpose and contract:** Return nearest-even(duration * scale_num / scale_den). Failure preserves *result for zero denominator, uint32 result overflow, or null result.

**Validation source:** tests/test_implementation.c

**Test-derived call:**
```c
pc_anim_duration_scale_u32(1u,1u,2u,&out)&&out==0u);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/chronovex/timing/pc_motion_duration_u32.h

**Public symbol:** pc_motion_duration_u32

**Implementation unit:** src/pc_motion_duration_u32.c

**Purpose and contract:** Elapsed modulo-2^32 timer ticks from start to now. The actual interval must be less than one complete timer cycle; multiple wraps are ambiguous.

**Validation source:** tests/driver.c

**Test-derived call:**
```c
pc_motion_duration_u32(s,n) != e);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/chronovex/timing/pc_time_deadline_from_now_u64.h

**Public symbol:** pc_time_deadline_from_now_u64

**Implementation unit:** src/pc_time_deadline_from_now_u64.c

**Purpose and contract:** Checked non-wrapping deadline = now + delay. False on null output or uint64 overflow; failure leaves *deadline unchanged.

**Validation source:** No enabled test source was found by symbol search.

**Test-derived call:**
```c
No test-derived call was found.;
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

## include/chronovex/timing/pc_time_until_deadline_i64.h

**Public symbol:** pc_time_until_deadline_i64

**Implementation unit:** src/pc_time_until_deadline_i64.c

**Purpose and contract:** Store signed deadline-now for uint64 timestamps. Positive means future, negative means overdue. False if result is outside int64 or output is null; failure leaves *remaining unchanged.

**Validation source:** tests/test_basic.c

**Test-derived call:**
```c
pc_time_until_deadline_i64(10,15,&out) && out==5);
```

**Integration notes:** use the exact fixed-width types and units shown in the declaration; check the boolean/status return before consuming an output pointer; do not assume overlap, rounding or overflow behavior beyond the contract above.

