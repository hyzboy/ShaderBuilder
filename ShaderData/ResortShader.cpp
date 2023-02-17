#include"ShaderGen/ShaderGenManager.h"
#include"ShaderData/ShaderDataManager.h"
#include<hgl/util/sort/Sort.h>
#include<hgl/log/LogInfo.h>

//template<> int Comparator<SDMPointer>::compare(const SDMPointer &a, const SDMPointer &b) const
//{
//    return a->GetStageBits()-b->GetStageBits();
//}

void ResortShader(ConvertMaterialData *cvd)
{
    LOG_INFO("Resort Shader.");

    ShaderDataInfo *prev,*cur,*next;

    auto *it=cvd->shader_map.GetDataList();

    const int count=cvd->shader_map.GetCount();

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
