#include <stdint.h>
#include "pc_sleep_break_even_u32.h"
int main(void)
{
    uint32_t out = 77u;
    if (!pc_sleep_break_even_u32(1u, 1000000u, &out) || out != 1u) return 1;
    if (!pc_sleep_break_even_u32(1u, 500000u, &out) || out != 2u) return 2;
    if (!pc_sleep_break_even_u32(0u, 1u, &out) || out != 0u) return 3;
    if (pc_sleep_break_even_u32(1u, 0u, &out) || out != 0u) return 4;
    out = 77u;
    if (pc_sleep_break_even_u32(UINT32_MAX, 1u, &out) || out != 77u) return 5;
    if (pc_sleep_break_even_u32(1u, 1u, 0)) return 6;
    return 0;
}
