#include"ShaderDataManager.h"

ShaderDataManager::ShaderDataManager(ShaderStageBits cur,ShaderStageBits prev,ShaderStageBits next)
{
    ssb=cur;

    stage_io.cur=cur;
    stage_io.prev=prev;
    stage_io.next=next;
}

void ShaderDataManager::Clear()
{
    stage_io.Clear();
        
    descriptor_list.Clear();
        
    ubo_list.Clear();
    object_list.Clear();
    
    const_value_list.Clear();
    subpass_input.Clear();
    
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

/**
* 添加一个描述符，如果它本身存在，则返回false
*/
bool ShaderDataManager::AddDescriptor(DescriptorSetsType set_type,ShaderDescriptor *new_sd)
{
    if(!new_sd)return(false);
    if(descriptor_name_list.Exist(new_sd->name))return(false);
    
    new_sd->set_type=set_type;
    new_sd->stage_flag=ssb;
    
    descriptor_list.Add(new_sd);
    descriptor_name_list.Add(new_sd->name);
    return(true);
}

bool ShaderDataManager::AddUBO(DescriptorSetsType type,ShaderUBOData *sd)
{
    RANGE_CHECK_RETURN_FALSE(type);
    if(!sd)return(false);

    if(!AddDescriptor(type,sd))return(false);

    ubo_list.Add(sd);
    return(true);
}

bool ShaderDataManager::AddObject(DescriptorSetsType type,ShaderObjectData *sd)
{
    RANGE_CHECK_RETURN_FALSE(type);
    if(!sd)return(false);
    
    if(!AddDescriptor(type,sd))return(false);

    object_list.Add(sd);
    return(true);
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
