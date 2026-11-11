#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "esp_cpu.h"
#include "esp_private/esp_clk.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "chronovex/timing/pc_anim_duration_scale_u32.h"
#include "chronovex/timing/pc_motion_duration_u32.h"
#include "chronovex/timing/pc_time_deadline_from_now_u64.h"
#include "chronovex/timing/pc_time_until_deadline_i64.h"

#define API_CHECK(index, condition) do { \
    if (!(condition)) { \
        api_ok[(index) - 1u] = false; \
        if (first_failed == 0u) first_failed = (index); \
    } \
} while (0)

typedef uint64_t (*bench_fn_t)(void);
static volatile uint32_t scale_duration = UINT32_C(1234567);
static volatile uint32_t scale_numerator = 3u;
static volatile uint32_t scale_denominator = 2u;
static volatile uint32_t motion_start = UINT32_C(0xfffffff0);
static volatile uint32_t motion_now = UINT32_C(0x2f);
static volatile uint64_t deadline_now = UINT64_C(0x0000000123456789);
static volatile uint64_t deadline_delay = UINT64_C(0x0000000013579bdf);
static volatile uint64_t until_now = UINT64_C(123456);
static volatile uint64_t until_deadline = UINT64_C(123999);
static volatile uint64_t benchmark_sink;
static const uint32_t repetitions = 1000u;
static const uint32_t sample_count = 31u;

static uint32_t passed;
static uint32_t first_failed;
static bool api_ok[4] = {true, true, true, true};

static bool (* volatile scale_api)(uint32_t, uint32_t, uint32_t, uint32_t *) = pc_anim_duration_scale_u32;
static bool scale_noop(uint32_t duration, uint32_t numerator, uint32_t denominator, uint32_t *result)
{
    (void)duration; (void)numerator; (void)denominator; *result = 0u; return true;
}
static bool (* volatile scale_baseline_api)(uint32_t, uint32_t, uint32_t, uint32_t *) = scale_noop;

static uint32_t (* volatile motion_api)(uint32_t, uint32_t) = pc_motion_duration_u32;
static uint32_t motion_noop(uint32_t start, uint32_t now) { (void)start; (void)now; return 0u; }
static uint32_t (* volatile motion_baseline_api)(uint32_t, uint32_t) = motion_noop;

static bool (* volatile deadline_api)(uint64_t, uint64_t, uint64_t *) = pc_time_deadline_from_now_u64;
static bool deadline_noop(uint64_t now, uint64_t delay, uint64_t *out) { (void)now; (void)delay; *out = 0u; return true; }
static bool (* volatile deadline_baseline_api)(uint64_t, uint64_t, uint64_t *) = deadline_noop;

static bool (* volatile until_api)(uint64_t, uint64_t, int64_t *) = pc_time_until_deadline_i64;
static bool until_noop(uint64_t now, uint64_t deadline, int64_t *out) { (void)now; (void)deadline; *out = 0; return true; }
static bool (* volatile until_baseline_api)(uint64_t, uint64_t, int64_t *) = until_noop;

static uint64_t bench_scale(void)
{
    uint32_t out = 0u;
    const bool ok = scale_api(scale_duration, scale_numerator, scale_denominator, &out);
    return ((uint64_t)ok << 32) | out;
}
static uint64_t bench_scale_baseline(void)
{
    uint32_t out = 0u;
    const bool ok = scale_baseline_api(scale_duration, scale_numerator, scale_denominator, &out);
    return ((uint64_t)ok << 32) | out;
}
static uint64_t bench_motion(void)
{
    return motion_api(motion_start, motion_now);
}
static uint64_t bench_motion_baseline(void)
{
    return motion_baseline_api(motion_start, motion_now);
}
static uint64_t bench_deadline(void)
{
    uint64_t out = 0u;
    const bool ok = deadline_api(deadline_now, deadline_delay, &out);
    return ((uint64_t)ok << 32) ^ out;
}
static uint64_t bench_deadline_baseline(void)
{
    uint64_t out = 0u;
    const bool ok = deadline_baseline_api(deadline_now, deadline_delay, &out);
    return ((uint64_t)ok << 32) ^ out;
}
static uint64_t bench_until(void)
{
    int64_t out = 0;
    const bool ok = until_api(until_now, until_deadline, &out);
    return ((uint64_t)ok << 32) ^ (uint64_t)out;
}
static uint64_t bench_until_baseline(void)
{
    int64_t out = 0;
    const bool ok = until_baseline_api(until_now, until_deadline, &out);
    return ((uint64_t)ok << 32) ^ (uint64_t)out;
}
static uint64_t bench_empty(void) { return UINT64_C(0x35); }

static uint32_t timed_batch(bench_fn_t fn)
{
    const uint32_t start = esp_cpu_get_cycle_count();
    uint32_t i;
    for (i = 0u; i < repetitions; ++i) benchmark_sink ^= fn();
    return esp_cpu_get_cycle_count() - start;
}
static int32_t median_i32(int32_t *values, uint32_t n)
{
    uint32_t i;
    for (i = 1u; i < n; ++i) {
        int32_t value = values[i];
        uint32_t j = i;
        while (j > 0u && values[j - 1u] > value) { values[j] = values[j - 1u]; --j; }
        values[j] = value;
    }
    return values[n / 2u];
}
static uint32_t median_u32(uint32_t *values, uint32_t n)
{
    uint32_t i;
    for (i = 1u; i < n; ++i) {
        uint32_t value = values[i];
        uint32_t j = i;
        while (j > 0u && values[j - 1u] > value) { values[j] = values[j - 1u]; --j; }
        values[j] = value;
    }
    return values[n / 2u];
}
static int32_t measure_pair(bench_fn_t actual, bench_fn_t baseline)
{
    int32_t deltas[31];
    uint32_t s;
    for (s = 0u; s < sample_count; ++s) {
        uint32_t a, b;
        if ((s & 1u) == 0u) { a = timed_batch(actual); b = timed_batch(baseline); }
        else { b = timed_batch(baseline); a = timed_batch(actual); }
        deltas[s] = (int32_t)((int64_t)a - (int64_t)b);
    }
    return median_i32(deltas, sample_count) / (int32_t)repetitions;
}
static uint32_t measure_empty_overhead(void)
{
    uint32_t values[31];
    uint32_t s;
    for (s = 0u; s < sample_count; ++s) values[s] = timed_batch(bench_empty);
    return median_u32(values, sample_count) / repetitions;
}

void app_main(void)
{
    uint32_t u32 = UINT32_C(0x5a5aa5a5);
    uint64_t u64 = UINT64_C(0x123456789abcdef0);
    int64_t i64 = INT64_C(0x123456789);

    vTaskDelay(pdMS_TO_TICKS(3000));

    API_CHECK(1u, pc_anim_duration_scale_u32(1u, 1u, 2u, &u32) && u32 == 0u &&
                   pc_anim_duration_scale_u32(3u, 1u, 2u, &u32) && u32 == 2u &&
                   pc_anim_duration_scale_u32(5u, 1u, 2u, &u32) && u32 == 2u &&
                   pc_anim_duration_scale_u32(7u, 1u, 2u, &u32) && u32 == 4u &&
                   pc_anim_duration_scale_u32(UINT32_MAX, UINT32_MAX, UINT32_MAX, &u32) && u32 == UINT32_MAX &&
                   pc_anim_duration_scale_u32(UINT32_MAX, 0u, UINT32_MAX, &u32) && u32 == 0u);
    u32 = UINT32_C(0x5a5aa5a5);
    API_CHECK(1u, !pc_anim_duration_scale_u32(1u, 1u, 0u, &u32) && u32 == UINT32_C(0x5a5aa5a5) &&
                   !pc_anim_duration_scale_u32(UINT32_MAX, UINT32_MAX, 1u, &u32) && u32 == UINT32_C(0x5a5aa5a5) &&
                   !pc_anim_duration_scale_u32(1u, 1u, 1u, NULL));

    API_CHECK(2u, pc_motion_duration_u32(UINT32_MAX - 2u, 3u) == 6u &&
                   pc_motion_duration_u32(17u, 17u) == 0u &&
                   pc_motion_duration_u32(0u, UINT32_MAX) == UINT32_MAX &&
                   pc_motion_duration_u32(5u, 4u) == UINT32_MAX);

    API_CHECK(3u, pc_time_deadline_from_now_u64(0u, 0u, &u64) && u64 == 0u &&
                   pc_time_deadline_from_now_u64(UINT64_MAX, 0u, &u64) && u64 == UINT64_MAX &&
                   pc_time_deadline_from_now_u64(UINT64_MAX - 1u, 1u, &u64) && u64 == UINT64_MAX);
    u64 = UINT64_C(0x123456789abcdef0);
    API_CHECK(3u, !pc_time_deadline_from_now_u64(UINT64_MAX, 1u, &u64) && u64 == UINT64_C(0x123456789abcdef0) &&
                   !pc_time_deadline_from_now_u64(0u, 0u, NULL));

    API_CHECK(4u, pc_time_until_deadline_i64(10u, 15u, &i64) && i64 == 5 &&
                   pc_time_until_deadline_i64(15u, 10u, &i64) && i64 == -5 &&
                   pc_time_until_deadline_i64(10u, 10u, &i64) && i64 == 0 &&
                   pc_time_until_deadline_i64(0u, (uint64_t)INT64_MAX, &i64) && i64 == INT64_MAX &&
                   pc_time_until_deadline_i64((uint64_t)INT64_MAX + 1u, 0u, &i64) && i64 == INT64_MIN &&
                   pc_time_until_deadline_i64(UINT64_MAX - 2u, UINT64_MAX, &i64) && i64 == 2 &&
                   pc_time_until_deadline_i64(UINT64_MAX, UINT64_MAX - 2u, &i64) && i64 == -2);
    i64 = INT64_C(0x123456789);
    API_CHECK(4u, !pc_time_until_deadline_i64(0u, (uint64_t)INT64_MAX + 1u, &i64) && i64 == INT64_C(0x123456789) &&
                   !pc_time_until_deadline_i64((uint64_t)INT64_MAX + 2u, 0u, &i64) && i64 == INT64_C(0x123456789) &&
                   !pc_time_until_deadline_i64(0u, 0u, NULL));

    {
        uint32_t i;
        for (i = 0u; i < 4u; ++i) if (api_ok[i]) ++passed;
    }
    if (passed == 4u) {
        const uint32_t overhead = measure_empty_overhead();
        const uint32_t cpu_mhz = esp_clk_cpu_freq() / 1000000u;
        static const bench_fn_t actual[4] = {bench_scale, bench_motion, bench_deadline, bench_until};
        static const bench_fn_t baseline[4] = {bench_scale_baseline, bench_motion_baseline, bench_deadline_baseline, bench_until_baseline};
        uint32_t i;
        (void)printf("CHRONOVEX_ALL4_PASS|passed=4/4|first_failed_api=0\n");
        (void)printf("CHRONOVEX_BENCH_META|cpu_mhz=%lu|repetitions=%lu|samples=%lu|empty_overhead_cycles_per_call=%lu|method=median(paired_actual_minus_noop)\n",
                     (unsigned long)cpu_mhz, (unsigned long)repetitions, (unsigned long)sample_count, (unsigned long)overhead);
        (void)printf("CHRONOVEX_BENCH_INPUTS|scale=1234567*3/2|motion_start=0xfffffff0|motion_now=0x2f|deadline_now=0x123456789|deadline_delay=0x13579bdf|until_now=123456|until_deadline=123999\n");
        for (i = 0u; i < 4u; ++i) {
            (void)printf("CHRONOVEX_CYCLE_MEDIAN|api_index=%lu|cycles=%ld\n", (unsigned long)(i + 1u), (long)measure_pair(actual[i], baseline[i]));
        }
        (void)printf("CHRONOVEX_BENCH_SINK|%llu\n", (unsigned long long)benchmark_sink);
        {
            const uint32_t min_free_words = uxTaskGetStackHighWaterMark(NULL);
            const uint32_t min_free_bytes = min_free_words * (uint32_t)sizeof(StackType_t);
            const uint32_t used_lower_bound = CONFIG_ESP_MAIN_TASK_STACK_SIZE - min_free_bytes;
            (void)printf("CHRONOVEX_STACK_WATERMARK|main_task_stack_bytes=%u|min_free_words=%lu|word_bytes=%u|observed_used_lower_bound_bytes=%lu\n",
                         (unsigned)CONFIG_ESP_MAIN_TASK_STACK_SIZE,
                         (unsigned long)min_free_words,
                         (unsigned)sizeof(StackType_t),
                         (unsigned long)used_lower_bound);
        }
    } else {
        (void)printf("CHRONOVEX_ALL4_FAIL|passed=%lu/4|first_failed_api=%lu\n", (unsigned long)passed, (unsigned long)first_failed);
    }
    for (;;) vTaskDelay(pdMS_TO_TICKS(1000));
}
