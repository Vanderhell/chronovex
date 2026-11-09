#include "pc_time_until_deadline_i64.h"

bool pc_time_until_deadline_i64(uint64_t now, uint64_t deadline,
                                int64_t *remaining)
{
    uint64_t magnitude;
    int64_t value;
    if (remaining == 0) return false;
    if (deadline >= now) {
        magnitude = deadline - now;
        if (magnitude > (uint64_t)INT64_MAX) return false;
        value = (int64_t)magnitude;
    } else {
        magnitude = now - deadline;
        if (magnitude > (uint64_t)INT64_MAX + UINT64_C(1)) return false;
        value = magnitude == (uint64_t)INT64_MAX + UINT64_C(1)
              ? INT64_MIN : -(int64_t)magnitude;
    }
    *remaining = value;
    return true;
}