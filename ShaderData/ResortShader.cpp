#include"ShaderData/ShaderDataInfo.h"
#include"ShaderData/ShaderDescriptorManager.h"
#include<hgl/util/sort/Sort.h>
#include<hgl/log/LogInfo.h>

void ResortShader(ShaderMap &shader_map)
{
    //ShaderMap使用ObjectMap保存,ObjectMap本质附带排序功能，所以这里无需再排序，直接设定prev,next即可

    LOG_INFO("Resort Shader.");

    ShaderDataInfo *prev,*cur,*next;

    auto *it=shader_map.GetDataList();

    const int count=shader_map.GetCount();

    for(int i=0; i<count; i++)
    {
        cur=(*it)->right;
        ++it;

        if(i>0)
            cur->sdm->SetPrevShader(prev->shader_stage_bit);

        if(i<count-1)
        {
            next=(*it)->right;
            cur->sdm->SetNextShader(next->shader_stage_bit);
        }

#ifdef _DEBUG
        cur->sdm->DebugOutput(i);
#endif//_DEBUG
            
        prev=cur;
    }
}
