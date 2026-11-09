#include "pc_anim_duration_scale_u32.h"

#include <stddef.h>

bool pc_anim_duration_scale_u32(uint32_t duration, uint32_t scale_num,
                                uint32_t scale_den, uint32_t *result)
{
    uint64_t product, quotient, remainder;

    if (result == NULL || scale_den == 0u) {
        return false;
    }

    product = (uint64_t)duration * scale_num;
    quotient = product / scale_den;
    remainder = product % scale_den;

    if (remainder > (uint64_t)scale_den - remainder ||
        (remainder == (uint64_t)scale_den - remainder &&
         (quotient & UINT64_C(1)) != 0u)) {
        ++quotient;
    }

    if (quotient > UINT32_MAX) {
        return false;
    }

    *result = (uint32_t)quotient;
    return true;
}