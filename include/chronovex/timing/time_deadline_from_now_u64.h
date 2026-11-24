#ifndef CHRONOVEX_TIME_DEADLINE_FROM_NOW_U64_H
#define CHRONOVEX_TIME_DEADLINE_FROM_NOW_U64_H
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
/* Checked non-wrapping deadline = now + delay. False on null output or
 * uint64 overflow; failure leaves *deadline unchanged. */
bool chronovex_time_deadline_from_now_u64(uint64_t now, uint64_t delay,
                                   uint64_t *deadline);
#ifdef __cplusplus
}
#endif

#endif
