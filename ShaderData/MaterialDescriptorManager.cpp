#include"MaterialDescriptorManager.h"

MaterialDescriptorManager::MaterialDescriptorManager()
{
    int set_type=(int)DescriptorSetsType::BEGIN_RANGE;

    for(auto &p:desc_set_array)
    {
        p.set_type=(DescriptorSetsType)set_type;

        ++set_type;

        p.set=-1;
        p.count=0;
    }
}

/**
* 添加一个描述符，如果它本身存在，则返回false
*/
const ShaderDescriptor *ShaderDescriptorSet::AddDescriptor(ShaderStageBits ssb,ShaderDescriptor *new_sd)
{
    ShaderDescriptor *sd;
    
    if(descriptor_map.Get(new_sd->name,sd))
    {
        sd->stage_flag|=ssb;
        return(sd);
    }
    else
    {
        new_sd->set_type=set_type;
        new_sd->stage_flag=ssb;

        descriptor_map.Add(new_sd->name, new_sd);

        count++;

        return(new_sd);
    }
}

const ShaderUBOData *MaterialDescriptorManager::AddUBO(ShaderStageBits ssb,DescriptorSetsType type,ShaderUBOData *sd)
{
    RANGE_CHECK_RETURN_NULLPTR(type);
    if(!sd)return(nullptr);

    ShaderDescriptorSet *sds=desc_set_array+(size_t)type;

    const ShaderDescriptor *obj=sds->AddDescriptor(ssb,sd);
    
    if(!obj)return(nullptr);

    sds->ubo_list.Add((ShaderUBOData *)obj);
    return((ShaderUBOData *)obj);
}

const ShaderObjectData *MaterialDescriptorManager::AddObject(ShaderStageBits ssb,DescriptorSetsType type,ShaderObjectData *sd)
{
    RANGE_CHECK_RETURN_NULLPTR(type);
    if(!sd)return(nullptr);
    
    ShaderDescriptorSet *sds=desc_set_array+(size_t)type;

    const ShaderDescriptor *obj=sds->AddDescriptor(ssb,sd);

    if(!obj)return(nullptr);

    sds->object_list.Add((ShaderObjectData *)obj);
    return((ShaderObjectData *)obj);
}

void MaterialDescriptorManager::Resort()
{
    //重新生成set/binding
    {
        int set=0;

        for(auto &p:desc_set_array)
        {
            if(p.count>0)
            {
                p.set=set;

                auto *sdp=p.descriptor_map.GetDataList();
                for(int i=0;i<p.descriptor_map.GetCount();i++)
                {
                    (*sdp)->right->set=set;
                    (*sdp)->right->binding=i;

                    ++sdp;
                }

                ++set;
            }
        }
    }
    
}