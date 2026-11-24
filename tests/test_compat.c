#include <stdint.h>

#include <chronovex/timing/pc_anim_duration_scale_u32.h>
#include <chronovex/timing/pc_motion_duration_u32.h>
#include <chronovex/timing/pc_time_deadline_from_now_u64.h>
#include <chronovex/timing/pc_time_until_deadline_i64.h>

int main(void)
{
    uint32_t scaled = 0u;
    uint64_t deadline = 0u;
    int64_t remaining = 0;

    if (!pc_anim_duration_scale_u32(10u, 3u, 2u, &scaled) || scaled != 15u) return 1;
    if (pc_motion_duration_u32(UINT32_MAX, 1u) != 2u) return 2;
    if (!pc_time_deadline_from_now_u64(40u, 2u, &deadline) || deadline != 42u) return 3;
    if (!pc_time_until_deadline_i64(40u, 42u, &remaining) || remaining != 2) return 4;
    return 0;
}
