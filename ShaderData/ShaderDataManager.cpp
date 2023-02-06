#include"ShaderDataManager.h"

ShaderDataManager::ShaderDataManager(ShaderStageBits cur,MaterialDescriptorManager *mdm)
{
    descriptor_set_manager=mdm;

    stage_io.cur=cur;
    stage_io.prev=ssbNone;
    stage_io.next=ssbNone;
    
    hgl_zero(push_constant);
}

namespace
{
    bool Find(ObjectList<ShaderStage> &ol,const ShaderStage *ss)
    {
        for(int i=0;i<ol.GetCount();i++)
            if(strcmp(ol[i]->name,ss->name)==0)
                return(true);

        return(false);
    }
}//namespace

bool ShaderDataManager::AddInput(ShaderStage *ss)
{
    if(!ss)return(false);

    if(Find(stage_io.input,ss))return(false);

    stage_io.input.Add(ss);
    return(true);
}

bool ShaderDataManager::AddOutput(ShaderStage *ss)
{
    if(!ss)return(false);

    if(Find(stage_io.output,ss))return(false);

    stage_io.output.Add(ss);
    return(true);
}

bool ShaderDataManager::AddUBO(DescriptorSetsType type,ShaderUBOData *sd)
{
    const ShaderUBOData *obj=descriptor_set_manager->AddUBO(stage_io.cur,type,sd);

    if(!obj)
        return(false);

    ubo_list.Add(obj);
    return obj;
}

bool ShaderDataManager::AddObject(DescriptorSetsType type,ShaderObjectData *sd)
{
    const ShaderObjectData *obj=descriptor_set_manager->AddObject(stage_io.cur,type,sd);

    if(!obj)
        return(false);

    object_list.Add(obj);
    return obj;
}

bool ShaderDataManager::AddConstValue(ShaderConstValue *sd)
{
    if(!sd)return(false);

    for(auto *p:const_value_list)
        if(p->name.Comp(sd->name)==0)
            return(false);

    const_value_list.Add(sd);
    return(true);
}

bool ShaderDataManager::AddSubpassInput(const UTF8String name,uint8_t index)
{
    for(auto *si:subpass_input)
    {
        if(si->input_attachment_index==index)return(false);
        if(si->name.Comp(name))return(false);
    }

    ShaderSubpassInput *ssi=new ShaderSubpassInput;

    ssi->name=name;
    ssi->input_attachment_index=index;

    subpass_input.Add(ssi);
    return(true);
}

void ShaderDataManager::SetPushConstant(const UTF8String name,uint8_t offset,uint8_t size)
{
    push_constant.name  =name;
    push_constant.offset=offset;
    push_constant.size  =size;
}

#ifdef _DEBUG
void ShaderDataManager::DebugOutput()
{
    AnsiString name=GetShaderStageName(stage_io.cur);
    AnsiString prev_name=GetShaderStageName(stage_io.prev);
    AnsiString next_name=GetShaderStageName(stage_io.next);
    
    LOG_INFO(name+" shader");
    LOG_INFO("\tprev next is "+prev_name);
    LOG_INFO("\tnext next is "+next_name);
    
}
#endif//_DEBUG