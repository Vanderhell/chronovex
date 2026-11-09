#include "primitivecore/arithmetic/bounded/integer/pc_time_deadline_from_now_u64.h"

bool pc_time_deadline_from_now_u64(uint64_t now, uint64_t delay,
                                   uint64_t *deadline)
{
    if (deadline == 0 || delay > UINT64_MAX - now) return false;
    *deadline = now + delay;
    return true;
}
