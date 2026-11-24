#include <stdint.h>

#include <chronovex/timing/motion_duration_u32.h>

int main(void)
{
    return chronovex_motion_duration_u32(UINT32_MAX - UINT32_C(2), UINT32_C(3)) == UINT32_C(6)
        ? 0 : 1;
}
