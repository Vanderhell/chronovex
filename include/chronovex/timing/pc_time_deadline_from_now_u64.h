#ifndef PC_PC_TIME_DEADLINE_FROM_NOW_U64_H
#define PC_PC_TIME_DEADLINE_FROM_NOW_U64_H
#include <stdbool.h>
#include <stdint.h>
/* Checked non-wrapping deadline = now + delay. False on null output or
 * uint64 overflow; failure leaves *deadline unchanged. */
bool pc_time_deadline_from_now_u64(uint64_t now, uint64_t delay,
                                   uint64_t *deadline);
#endif
