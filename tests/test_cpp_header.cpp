#include <stdint.h>

#include <chronovex/timing/anim_duration_scale_u32.h>
#include <chronovex/timing/motion_duration_u32.h>
#include <chronovex/timing/time_deadline_from_now_u64.h>
#include <chronovex/timing/time_until_deadline_i64.h>

int main()
{
    uint32_t scaled = 0u;
    uint64_t deadline = 0u;
    int64_t remaining = 0;

    if (!chronovex_anim_duration_scale_u32(10u, 3u, 2u, &scaled) || scaled != 15u) return 1;
    if (chronovex_motion_duration_u32(~uint32_t(0), 1u) != 2u) return 2;
    if (!chronovex_time_deadline_from_now_u64(40u, 2u, &deadline) || deadline != 42u) return 3;
    if (!chronovex_time_until_deadline_i64(40u, 42u, &remaining) || remaining != 2) return 4;
    return 0;
}
