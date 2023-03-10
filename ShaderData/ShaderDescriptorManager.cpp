#include"ShaderDescriptorManager.h"

ShaderDescriptorManager::ShaderDescriptorManager(VkShaderStageFlagBits cur,MaterialDescriptorManager *mdm)
{
    material_descriptor_manager=mdm;

    stage_io.cur=cur;
    stage_io.prev=(VkShaderStageFlagBits)0;
    stage_io.next=(VkShaderStageFlagBits)0;
    
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

bool ShaderDescriptorManager::AddInput(ShaderStage *ss)
{
    if(!ss)return(false);

    if(Find(stage_io.input,ss))return(false);

    ss->location=stage_io.input.GetCount();
    stage_io.input.Add(ss);
    return(true);
}

bool ShaderDescriptorManager::AddOutput(ShaderStage *ss)
{
    if(!ss)return(false);

    if(Find(stage_io.output,ss))return(false);
    
    ss->location=stage_io.output.GetCount();
    stage_io.output.Add(ss);
    return(true);
}

bool ShaderDescriptorManager::AddUBO(DescriptorSetType type,UBODescriptor *sd)
{
    const UBODescriptor *obj=material_descriptor_manager->AddUBO(stage_io.cur,type,sd);

    if(!obj)
        return(false);

    ubo_list.Add(obj);
    return obj;
}

bool ShaderDescriptorManager::AddObject(DescriptorSetType type,ShaderObjectData *sd)
{
    const ShaderObjectData *obj=material_descriptor_manager->AddObject(stage_io.cur,type,sd);

    if(!obj)
        return(false);

    object_list.Add(obj);
    return obj;
}

bool ShaderDescriptorManager::AddConstValue(ConstValueDescriptor *sd)
{
    if(!sd)return(false);

    for(auto *p:const_value_list)
        if(p->name.Comp(sd->name)==0)
            return(false);

    sd->constant_id=const_value_list.Add(sd);
    return(true);
}

bool ShaderDescriptorManager::AddSubpassInput(const UTF8String name,uint8_t index)
{
    for(auto *si:subpass_input)
    {
        if(si->input_attachment_index==index)return(false);
        if(si->name.Comp(name))return(false);
    }

    SubpassInputDescriptor *ssi=new SubpassInputDescriptor;

    ssi->name=name;
    ssi->input_attachment_index=index;

    subpass_input.Add(ssi);
    return(true);
}

void ShaderDescriptorManager::SetPushConstant(const UTF8String name,uint8_t offset,uint8_t size)
{
    push_constant.name  =name;
    push_constant.offset=offset;
    push_constant.size  =size;
}

#ifdef _DEBUG
void ShaderDescriptorManager::DebugOutput(int index)
{
    UTF8String name=GetShaderStageName(stage_io.cur);
    UTF8String prev_name=GetShaderStageName(stage_io.prev);
    UTF8String next_name=GetShaderStageName(stage_io.next);
    
    LOG_INFO(UTF8String::numberOf(index)+": "+name+" shader");
    LOG_INFO("\tprev next is "+prev_name);
    LOG_INFO("\tnext next is "+next_name);

    if(stage_io.input.GetCount()>0)
    {
        LOG_INFO("\tStage Input "+UTF8String::numberOf(stage_io.input.GetCount()));

        for(auto *ss:stage_io.input)
            LOG_INFO("\t\tlayout(location="+UTF8String::numberOf(ss->location)+") in "+UTF8String(GetShaderStageTypeName(ss))+"\t"+UTF8String(ss->name));
    }

    if(stage_io.output.GetCount()>0)
    {
        LOG_INFO("\tStage Output "+UTF8String::numberOf(stage_io.output.GetCount()));

        for(auto *ss:stage_io.output)
            LOG_INFO("\t\tlayout(location="+UTF8String::numberOf(ss->location)+") out "+UTF8String(GetShaderStageTypeName(ss))+"\t"+UTF8String(ss->name));
    }
    
    if(ubo_list.GetCount()>0)
    {
        LOG_INFO("\tUBO "+UTF8String::numberOf(ubo_list.GetCount()));
        
        for(auto *ubo:ubo_list)
            LOG_INFO("\t\tlayout(set="+UTF8String::numberOf(ubo->set)+",binding="+UTF8String::numberOf(ubo->binding)+") uniform "+ubo->type+"\t"+ubo->type);
    }

    if(object_list.GetCount()>0)
    {
        LOG_INFO("\tObject "+UTF8String::numberOf(object_list.GetCount()));

        for(auto *ubo:ubo_list)
            LOG_INFO("\t\tlayout(set="+UTF8String::numberOf(ubo->set)+",binding="+UTF8String::numberOf(ubo->binding)+") uniform "+ubo->type+"\t"+ubo->type);
    }
}
#endif//_DEBUG