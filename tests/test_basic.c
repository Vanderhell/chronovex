#include <assert.h>
#include <stdint.h>
#include "pc_time_until_deadline_i64.h"
int main(void)
{
    int64_t out=77;
    assert(pc_time_until_deadline_i64(10,15,&out) && out==5);
    assert(pc_time_until_deadline_i64(15,10,&out) && out==-5);
    assert(pc_time_until_deadline_i64(10,10,&out) && out==0);
    assert(pc_time_until_deadline_i64(0,(uint64_t)INT64_MAX,&out) && out==INT64_MAX);
    assert(pc_time_until_deadline_i64((uint64_t)INT64_MAX+1u,0,&out) && out==INT64_MIN);
    out=77;
    assert(!pc_time_until_deadline_i64(0,(uint64_t)INT64_MAX+1u,&out) && out==77);
    assert(!pc_time_until_deadline_i64((uint64_t)INT64_MAX+2u,0,&out) && out==77);
    assert(!pc_time_until_deadline_i64(0,0,0));
    return 0;
}