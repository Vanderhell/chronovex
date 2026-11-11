#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "chronovex/timing/pc_time_until_deadline_i64.h"
#define CHECK(c) do { if (!(c)) { (void)fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#c); return EXIT_FAILURE; } } while (0)
int main(void)
{
    int64_t out = 77;
    CHECK(pc_time_until_deadline_i64(10u,15u,&out) && out==5);
    CHECK(pc_time_until_deadline_i64(15u,10u,&out) && out==-5);
    CHECK(pc_time_until_deadline_i64(10u,10u,&out) && out==0);
    CHECK(pc_time_until_deadline_i64(0u,(uint64_t)INT64_MAX,&out) && out==INT64_MAX);
    CHECK(pc_time_until_deadline_i64((uint64_t)INT64_MAX+1u,0u,&out) && out==INT64_MIN);
    out=77;
    CHECK(!pc_time_until_deadline_i64(0u,(uint64_t)INT64_MAX+1u,&out) && out==77);
    CHECK(!pc_time_until_deadline_i64((uint64_t)INT64_MAX+2u,0u,&out) && out==77);
    CHECK(!pc_time_until_deadline_i64(0u,0u,NULL));
    return EXIT_SUCCESS;
}
