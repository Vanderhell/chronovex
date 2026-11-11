#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "chronovex/timing/pc_anim_duration_scale_u32.h"
#include "chronovex/timing/pc_motion_duration_u32.h"
#include "chronovex/timing/pc_time_deadline_from_now_u64.h"
#include "chronovex/timing/pc_time_until_deadline_i64.h"

#define CHECK(condition) do { \
    if (!(condition)) { \
        (void)fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, #condition); \
        return EXIT_FAILURE; \
    } \
} while (0)

static uint64_t rng_state = UINT64_C(0x93b6a4d52f1078c1);

static uint64_t next_random(void)
{
    uint64_t x = rng_state;
    x ^= x << 13;
    x ^= x >> 7;
    x ^= x << 17;
    rng_state = x;
    return x;
}

/* Independent exact product using base-2^16 limbs, then binary long division. */
static bool reference_scale(uint32_t duration, uint32_t numerator,
                           uint32_t denominator, uint32_t *result)
{
    uint32_t a[2] = {duration & UINT32_C(0xffff), duration >> 16};
    uint32_t b[2] = {numerator & UINT32_C(0xffff), numerator >> 16};
    uint32_t limb[4] = {0u, 0u, 0u, 0u};
    uint64_t carry = 0u;
    uint64_t remainder = 0u;
    uint64_t quotient = 0u;
    unsigned k;

    if (denominator == 0u || result == NULL) return false;
    for (k = 0u; k < 4u; ++k) {
        uint64_t sum = carry;
        unsigned i;
        for (i = 0u; i < 2u; ++i) {
            if (k >= i && k - i < 2u) sum += (uint64_t)a[i] * b[k - i];
        }
        limb[k] = (uint32_t)(sum & UINT64_C(0xffff));
        carry = sum >> 16;
    }
    for (k = 64u; k > 0u; --k) {
        const unsigned bit = k - 1u;
        const uint64_t product_bit = (limb[bit / 16u] >> (bit % 16u)) & 1u;
        remainder = remainder * 2u + product_bit;
        if (remainder >= denominator) {
            remainder -= denominator;
            quotient |= UINT64_C(1) << bit;
        }
    }
    if (remainder > (uint64_t)denominator - remainder ||
        (remainder == (uint64_t)denominator - remainder && (quotient & 1u) != 0u)) {
        ++quotient;
    }
    if (quotient > UINT32_MAX) return false;
    *result = (uint32_t)quotient;
    return true;
}

static bool reference_deadline(uint64_t now, uint64_t delay, uint64_t *out)
{
    const uint64_t low_sum = (uint64_t)(uint32_t)now + (uint32_t)delay;
    const uint64_t high_sum = (now >> 32) + (delay >> 32) + (low_sum >> 32);
    if (high_sum > UINT32_MAX) return false;
    *out = (high_sum << 32) | (uint32_t)low_sum;
    return true;
}

static bool reference_until(uint64_t now, uint64_t deadline, int64_t *out)
{
    uint64_t magnitude;
    if (deadline >= now) {
        const uint64_t low = (uint64_t)(uint32_t)deadline - (uint32_t)now;
        const uint64_t borrow = (uint32_t)deadline < (uint32_t)now ? 1u : 0u;
        const uint64_t high = (deadline >> 32) - (now >> 32) - borrow;
        magnitude = (high << 32) | (uint32_t)low;
        if (high > UINT32_C(0x7fffffff) ||
            (high == UINT32_C(0x7fffffff) && (uint32_t)low > UINT32_MAX)) return false;
        *out = (int64_t)magnitude;
        return true;
    }
    {
        const uint64_t low = (uint64_t)(uint32_t)now - (uint32_t)deadline;
        const uint64_t borrow = (uint32_t)now < (uint32_t)deadline ? 1u : 0u;
        const uint64_t high = (now >> 32) - (deadline >> 32) - borrow;
        magnitude = (high << 32) | (uint32_t)low;
        if (high > UINT32_C(0x80000000) ||
            (high == UINT32_C(0x80000000) && (uint32_t)low != 0u)) return false;
        *out = -(int64_t)(magnitude - 1u) - 1;
        return true;
    }
}

static uint32_t reference_elapsed(uint32_t start, uint32_t now)
{
    const uint64_t modulus = UINT64_C(1) << 32;
    return (uint32_t)((now >= start) ? (uint64_t)(now - start)
                                      : modulus - start + now);
}

int main(void)
{
    uint32_t u32, expected_u32;
    uint64_t u64, expected_u64;
    int64_t i64, expected_i64;
    unsigned i;

    /* Animation scaling boundaries, rounding ties, invalid inputs, preservation. */
    CHECK(pc_anim_duration_scale_u32(1u, 1u, 2u, &u32) && u32 == 0u);
    CHECK(pc_anim_duration_scale_u32(3u, 1u, 2u, &u32) && u32 == 2u);
    CHECK(pc_anim_duration_scale_u32(5u, 1u, 2u, &u32) && u32 == 2u);
    CHECK(pc_anim_duration_scale_u32(7u, 1u, 2u, &u32) && u32 == 4u);
    CHECK(pc_anim_duration_scale_u32(UINT32_MAX, UINT32_MAX, UINT32_MAX, &u32) && u32 == UINT32_MAX);
    CHECK(pc_anim_duration_scale_u32(UINT32_MAX, 0u, UINT32_MAX, &u32) && u32 == 0u);
    u32 = 0x12345678u;
    CHECK(!pc_anim_duration_scale_u32(1u, 1u, 0u, &u32) && u32 == 0x12345678u);
    CHECK(!pc_anim_duration_scale_u32(UINT32_MAX, UINT32_MAX, 1u, &u32) && u32 == 0x12345678u);
    CHECK(!pc_anim_duration_scale_u32(1u, 1u, 1u, NULL));

    /* Modular elapsed-time boundary vectors. */
    CHECK(pc_motion_duration_u32(UINT32_MAX - 2u, 3u) == 6u);
    CHECK(pc_motion_duration_u32(17u, 17u) == 0u);
    CHECK(pc_motion_duration_u32(0u, UINT32_MAX) == UINT32_MAX);
    CHECK(pc_motion_duration_u32(5u, 4u) == UINT32_MAX);
    CHECK(pc_motion_duration_u32(0u, 0u) == 0u); /* also what one full wrap aliases to */

    /* Checked absolute timestamp addition. */
    CHECK(pc_time_deadline_from_now_u64(0u, 0u, &u64) && u64 == 0u);
    CHECK(pc_time_deadline_from_now_u64(UINT64_MAX, 0u, &u64) && u64 == UINT64_MAX);
    CHECK(pc_time_deadline_from_now_u64(UINT64_MAX - 1u, 1u, &u64) && u64 == UINT64_MAX);
    u64 = UINT64_C(0x123456789abcdef0);
    CHECK(!pc_time_deadline_from_now_u64(UINT64_MAX, 1u, &u64) && u64 == UINT64_C(0x123456789abcdef0));
    CHECK(!pc_time_deadline_from_now_u64(1u, UINT64_MAX, &u64) && u64 == UINT64_C(0x123456789abcdef0));
    CHECK(!pc_time_deadline_from_now_u64(0u, 0u, NULL));

    /* Signed deadline difference representability boundaries. */
    CHECK(pc_time_until_deadline_i64(10u, 15u, &i64) && i64 == 5);
    CHECK(pc_time_until_deadline_i64(15u, 10u, &i64) && i64 == -5);
    CHECK(pc_time_until_deadline_i64(10u, 10u, &i64) && i64 == 0);
    CHECK(pc_time_until_deadline_i64(0u, (uint64_t)INT64_MAX, &i64) && i64 == INT64_MAX);
    CHECK(pc_time_until_deadline_i64((uint64_t)INT64_MAX + 1u, 0u, &i64) && i64 == INT64_MIN);
    CHECK(pc_time_until_deadline_i64(UINT64_MAX - 2u, UINT64_MAX, &i64) && i64 == 2);
    CHECK(pc_time_until_deadline_i64(UINT64_MAX, UINT64_MAX - 2u, &i64) && i64 == -2);
    i64 = INT64_C(0x123456789);
    CHECK(!pc_time_until_deadline_i64(0u, (uint64_t)INT64_MAX + 1u, &i64) && i64 == INT64_C(0x123456789));
    CHECK(!pc_time_until_deadline_i64((uint64_t)INT64_MAX + 2u, 0u, &i64) && i64 == INT64_C(0x123456789));
    CHECK(!pc_time_until_deadline_i64(0u, 0u, NULL));

    /* 100,000 deterministic differential cases for every applicable API. */
    for (i = 0u; i < 100000u; ++i) {
        uint32_t duration = (uint32_t)next_random();
        uint32_t numerator = (uint32_t)(next_random() >> 32);
        uint32_t denominator = (uint32_t)next_random();
        bool expected_ok, actual_ok;
        u32 = UINT32_C(0xa5a55a5a);
        expected_u32 = UINT32_C(0x5a5aa5a5);
        expected_ok = reference_scale(duration, numerator, denominator, &expected_u32);
        actual_ok = pc_anim_duration_scale_u32(duration, numerator, denominator, &u32);
        CHECK(actual_ok == expected_ok);
        CHECK(actual_ok ? u32 == expected_u32 : u32 == UINT32_C(0xa5a55a5a));

        {
            uint32_t start = (uint32_t)next_random();
            uint32_t now = (uint32_t)next_random();
            CHECK(pc_motion_duration_u32(start, now) == reference_elapsed(start, now));
        }
        {
            uint64_t now = next_random();
            uint64_t delay = next_random();
            u64 = UINT64_C(0x1122334455667788);
            expected_u64 = UINT64_C(0x8877665544332211);
            expected_ok = reference_deadline(now, delay, &expected_u64);
            actual_ok = pc_time_deadline_from_now_u64(now, delay, &u64);
            CHECK(actual_ok == expected_ok);
            CHECK(actual_ok ? u64 == expected_u64 : u64 == UINT64_C(0x1122334455667788));
        }
        {
            uint64_t now = next_random();
            uint64_t deadline = next_random();
            i64 = INT64_C(0x13579bdf);
            expected_i64 = INT64_C(0x2468ace0);
            expected_ok = reference_until(now, deadline, &expected_i64);
            actual_ok = pc_time_until_deadline_i64(now, deadline, &i64);
            CHECK(actual_ok == expected_ok);
            CHECK(actual_ok ? i64 == expected_i64 : i64 == INT64_C(0x13579bdf));
        }
    }
    (void)puts("chronovex audit differential: 100000 cases per API PASS");
    return EXIT_SUCCESS;
}
