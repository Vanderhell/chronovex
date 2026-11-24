#include <stdint.h>

#include <chronovex/timing/motion_duration_u32.h>
#include <chronovex/timing/time_deadline_from_now_u64.h>

int main(void)
{
    const uint32_t elapsed = chronovex_motion_duration_u32(UINT32_MAX - UINT32_C(2), UINT32_C(3));
    uint64_t deadline = 0u;

    if (elapsed != 6u) return 1;
    if (!chronovex_time_deadline_from_now_u64(UINT64_C(1000), UINT64_C(250), &deadline)) return 2;
    return deadline == UINT64_C(1250) ? 0 : 3;
}
