#include "primitivecore/arithmetic/bounded/integer/pc_motion_duration_u32.h"
uint32_t pc_motion_duration_u32(uint32_t start, uint32_t now)
{
    return now - start;
}
