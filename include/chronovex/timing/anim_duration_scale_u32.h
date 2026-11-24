#ifndef CHRONOVEX_ANIM_DURATION_SCALE_U32_H
#define CHRONOVEX_ANIM_DURATION_SCALE_U32_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* Return nearest-even(duration * scale_num / scale_den). Failure preserves
 * *result for zero denominator, uint32 result overflow, or null result. */
bool chronovex_anim_duration_scale_u32(uint32_t duration, uint32_t scale_num,
                                uint32_t scale_den, uint32_t *result);

#ifdef __cplusplus
}
#endif

#endif
