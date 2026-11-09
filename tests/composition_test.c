#include "pc_ack_delay_u32.h"

#include <stddef.h>
#include <stdint.h>

#define CHECK(c) do { if (!(c)) return __LINE__; } while (0)

int main(void)
{
    uint32_t delay = 77u;
    CHECK(pc_ack_delay_u32(5u, 3u, &delay) && delay == 40u);
    CHECK(pc_ack_delay_u32(1u, 20u, &delay) && delay == UINT32_C(1048576));
    delay = 77u;
    CHECK(!pc_ack_delay_u32(UINT32_MAX, 1u, &delay) && delay == 77u);
    CHECK(!pc_ack_delay_u32(1u, 21u, &delay) && delay == 77u);
    CHECK(!pc_ack_delay_u32(1u, 1u, NULL));
    return 0;
}