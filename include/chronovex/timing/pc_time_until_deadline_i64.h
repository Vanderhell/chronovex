#ifndef PC_PC_TIME_UNTIL_DEADLINE_I64_H
#define PC_PC_TIME_UNTIL_DEADLINE_I64_H
#include <stdbool.h>
#include <stdint.h>
/* Store signed deadline-now for uint64 timestamps. Positive means future,
 * negative means overdue. False if result is outside int64 or output is null;
 * failure leaves *remaining unchanged. */
bool pc_time_until_deadline_i64(uint64_t now, uint64_t deadline,
                                int64_t *remaining);
#endif
