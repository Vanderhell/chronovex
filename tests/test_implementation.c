#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "chronovex/timing/pc_anim_duration_scale_u32.h"
#define CHECK(c) do { if (!(c)) { (void)fprintf(stderr,"FAIL %s:%d: %s\n",__FILE__,__LINE__,#c); return EXIT_FAILURE; } } while (0)
int main(void)
{
    uint32_t out=17u;
    CHECK(pc_anim_duration_scale_u32(1u,1u,2u,&out)&&out==0u);
    CHECK(pc_anim_duration_scale_u32(3u,1u,2u,&out)&&out==2u);
    CHECK(pc_anim_duration_scale_u32(UINT32_MAX,1u,1u,&out)&&out==UINT32_MAX);
    out=17u;
    CHECK(!pc_anim_duration_scale_u32(1u,1u,0u,&out)&&out==17u);
    CHECK(!pc_anim_duration_scale_u32(UINT32_MAX,2u,1u,&out)&&out==17u);
    CHECK(!pc_anim_duration_scale_u32(1u,1u,1u,NULL));
    return EXIT_SUCCESS;
}
