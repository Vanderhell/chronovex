#include <stdint.h>
#include <stdio.h>
#include "chronovex_motion_duration_u32.h"
int main(void) { uint32_t s,n,e; while (scanf("%u%u%u", &s,&n,&e) == 3) if (chronovex_motion_duration_u32(s,n) != e) return 1; return 0; }