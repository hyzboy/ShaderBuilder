#include"ShaderData/ShaderDataInfo.h"
#include"ShaderData/ShaderDescriptorManager.h"
#include<hgl/util/sort/Sort.h>
#include<hgl/log/LogInfo.h>

void ResortShader(ShaderMap &shader_map)
{
    //ShaderMapwgk使用ObjectMap,其本质附带排序功能，所以这里无需再排序，仅设定prev,next即可

    LOG_INFO("Resort Shader.");

    ShaderDataInfo *prev,*cur,*next;

    auto *it=shader_map.GetDataList();

    const int count=shader_map.GetCount();

    for(int i=0; i<count; i++)
    {
        cur=(*it)->right;
        ++it;

        if(i>0)
            cur->shader_data_manager->SetPrevShader(prev->shader_stage_bit);

        if(i<count-1)
        {
            next=(*it)->right;
            cur->shader_data_manager->SetNextShader(next->shader_stage_bit);
        }

#ifdef _DEBUG
        cur->shader_data_manager->DebugOutput(i);
#endif//_DEBUG
            
        prev=cur;
    }
}
