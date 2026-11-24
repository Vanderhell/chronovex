#include <chronovex/timing/motion_duration_u32.h>
uint32_t chronovex_motion_duration_u32(uint32_t start, uint32_t now)
{
    return now - start;
}