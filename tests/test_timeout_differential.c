#include <stdint.h>
#include <stddef.h>
#include "../include/pc_ble_supervision_timeout_u32.h"

static uint32_t next(uint32_t *state)
{
    uint32_t x = *state;
    x ^= x << 13;
    x ^= x >> 17;
    x ^= x << 5;
    *state = x;
    return x;
}

static int reference(uint32_t interval, uint16_t latency, uint16_t factor,
                     uint32_t *out)
{
    uint64_t scale, bound, field;
    if (interval < 375u || interval > 4000000u || interval % 125u != 0u ||
        factor == 0u || factor > 500u) return 0;
    scale = (uint64_t)factor * ((uint64_t)latency + 1u);
    if (scale > 500u) return 0;
    bound = (uint64_t)interval * scale * 2u;
    field = bound / 10000u + 1u;
    if (field < 10u) field = 10u;
    if (field > 3200u) return 0;
    *out = (uint32_t)field;
    return 1;
}

int main(void)
{
    uint32_t seed = UINT32_C(0x3142B1E), i;
    for (i = 0; i < 1000000u; ++i) {
        uint32_t interval = (3u + next(&seed) % 31998u) * 125u;
        uint16_t factor = (uint16_t)(1u + next(&seed) % 500u);
        uint16_t latency = (uint16_t)(next(&seed) % (500u / factor));
        uint32_t expected = UINT32_C(0xA5A5A5A5), actual = expected;
        int ref_ok = reference(interval, latency, factor, &expected);
        int ok = pc_ble_supervision_timeout_u32(interval, latency, factor, &actual);
        if (ok != ref_ok || (ok && actual != expected)) return 1;
        if (!ok && actual != UINT32_C(0xA5A5A5A5)) return 2;
    }
    {
        uint32_t out = 0;
        if (!pc_ble_supervision_timeout_u32(50000u, 0u, 1u, &out) || out != 11u) return 3;
        if (!pc_ble_supervision_timeout_u32(3999875u, 0u, 4u, &out) || out != 3200u) return 4;
        if (pc_ble_supervision_timeout_u32(4000000u, 0u, 4u, &out)) return 5;
        if (pc_ble_supervision_timeout_u32(375u, 500u, 1u, &out)) return 6;
        if (pc_ble_supervision_timeout_u32(375u, 0u, 1u, NULL)) return 7;
        if (pc_ble_supervision_timeout_u32(4000000u, 0u, 500u, &out)) return 8;
    }
    return 0;
}