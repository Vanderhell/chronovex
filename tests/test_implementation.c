#include "pc_anim_duration_scale_u32.h"
#include <assert.h>
#include <stdint.h>

int main(void)
{
    uint32_t out;
    assert(pc_anim_duration_scale_u32(1u,1u,2u,&out)&&out==0u);
    assert(pc_anim_duration_scale_u32(3u,1u,2u,&out)&&out==2u);
    assert(pc_anim_duration_scale_u32(UINT32_MAX,1u,1u,&out)&&out==UINT32_MAX);
    out=17u;
    assert(!pc_anim_duration_scale_u32(1u,1u,0u,&out)&&out==17u);
    assert(!pc_anim_duration_scale_u32(UINT32_MAX,2u,1u,&out)&&out==17u);
    assert(!pc_anim_duration_scale_u32(1u,1u,1u,0));
    return 0;
}