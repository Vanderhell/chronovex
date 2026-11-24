#ifndef CHRONOVEX_MOTION_DURATION_U32_H
#define CHRONOVEX_MOTION_DURATION_U32_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/* Elapsed modulo-2^32 timer ticks from start to now. The actual interval
 * must be less than one complete timer cycle; multiple wraps are ambiguous. */
uint32_t chronovex_motion_duration_u32(uint32_t start, uint32_t now);
#ifdef __cplusplus
}
#endif

#endif
