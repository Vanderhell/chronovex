#include "roundmath/pc_rtc_day_of_year_u32.h"
int main(void) { return pc_rtc_day_of_year_u32(2000u, 12u, 31u) == 366u && pc_rtc_day_of_year_u32(1900u, 2u, 29u) == 0u ? 0 : 1; }