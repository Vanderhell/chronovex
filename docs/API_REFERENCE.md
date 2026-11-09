# Chronovex API reference

Complete public-header inventory generated from the current source tree. Headers remain authoritative.

## include/chronovex/timing/pc_anim_duration_scale_u32.h

Public API: pc_anim_duration_scale_u32

Contract: Return nearest-even(duration * scale_num / scale_den). Failure preserves *result for zero denominator, uint32 result overflow, or null result.

Header path: include/chronovex/timing/pc_anim_duration_scale_u32.h

## include/chronovex/timing/pc_motion_duration_u32.h

Public API: pc_motion_duration_u32

Contract: Elapsed modulo-2^32 timer ticks from start to now. The actual interval must be less than one complete timer cycle; multiple wraps are ambiguous.

Header path: include/chronovex/timing/pc_motion_duration_u32.h

## include/chronovex/timing/pc_time_deadline_from_now_u64.h

Public API: pc_time_deadline_from_now_u64

Contract: Checked non-wrapping deadline = now + delay. False on null output or uint64 overflow; failure leaves *deadline unchanged.

Header path: include/chronovex/timing/pc_time_deadline_from_now_u64.h

## include/chronovex/timing/pc_time_until_deadline_i64.h

Public API: pc_time_until_deadline_i64

Contract: Store signed deadline-now for uint64 timestamps. Positive means future, negative means overdue. False if result is outside int64 or output is null; failure leaves *remaining unchanged.

Header path: include/chronovex/timing/pc_time_until_deadline_i64.h
