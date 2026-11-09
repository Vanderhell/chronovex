#ifndef PRIMITIVECORE_PC_MOTION_DURATION_U32_H
#define PRIMITIVECORE_PC_MOTION_DURATION_U32_H
#include <stdint.h>
/* Elapsed modulo-2^32 timer ticks from start to now. The actual interval
 * must be less than one complete timer cycle; multiple wraps are ambiguous. */
uint32_t pc_motion_duration_u32(uint32_t start, uint32_t now);
#endif
