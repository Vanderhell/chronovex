#include "roundmath/pc_rtc_time_valid_bool.h"
int main(void)
{
    unsigned h,m,s;
    for(h=0;h<256;++h) for(m=0;m<256;++m) for(s=0;s<256;++s)
        if (pc_rtc_time_valid_bool((uint8_t)h,(uint8_t)m,(uint8_t)s) != (h<24u && m<60u && s<60u)) return 1;
    return 0;
}