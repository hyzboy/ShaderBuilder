#include"ShaderData/ShaderDataManager.h"
#include<hgl/util/sort/Sort.h>
#include<hgl/log/LogInfo.h>

using SDMPointer=ShaderDataManager *;

template<> int Comparator<SDMPointer>::compare(const SDMPointer &a, const SDMPointer &b) const
{
    return a->GetStageBits()-b->GetStageBits();
}

void ResortShaderDescriptor(List<SDMPointer> shader_list)
{
    {
        Comparator<SDMPointer> comp;

        Sort(shader_list.GetData(), shader_list.GetCount(), &comp);

        SDMPointer *prev,*cur,*next;

        prev=nullptr;
        cur=shader_list.GetData();
        next=++cur;

        for(int i=0; i<shader_list.GetCount(); i++)
        {
            if(i>0)
                (*cur)->SetPrevShader((*prev)->GetStageBits());

            if(i<shader_list.GetCount()-1)
                (*cur)->SetNextShader((*next)->GetStageBits());

            prev=cur;
            cur=next;
            ++next;
        }
    }
    
    {
        LOG_INFO("Resort Shader.");

        SDMPointer *p=shader_list.GetData();

        for(int i=0;i<shader_list.GetCount();i++)
        {
            AnsiString name=GetShaderStageName((*p)->GetStageBits());
            
            LOG_INFO("Shader["+AnsiString::numberOf(i)+"]:"+name);

            ++p;
        }
    }
}
