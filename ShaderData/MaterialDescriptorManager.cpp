#include"MaterialDescriptorManager.h"

/**
* 根据名称获取一个描述符
*/
ShaderDescriptor *MaterialDescriptorManager::GetDescriptor(const char *name)
{
    if(!name||!*name)
        return(nullptr);

    for(ShaderDescriptor *sd:descriptor_list)
    {
        if(strcmp(sd->name,name)==0)
            return sd;
    }

    return(nullptr);
}
        
/**
* 添加一个描述符，如果它本身存在，则返回false
*/
bool MaterialDescriptorManager::AddDescriptor(ShaderStageBits bit,DescriptorSetsType set_type,ShaderDescriptor *new_sd)
{
    ShaderDescriptor *sd=GetDescriptor(new_sd->name);

    if(sd)
    {
        sd->stage_flag|=bit;
        return(false);
    }
            
    new_sd->set_type=set_type;
    new_sd->stage_flag=bit;
    descriptor_list.Add(new_sd);
    return(true);
}

void MaterialDescriptorManager::Clear()
{
    stage_io_map.Clear();
    descriptor_list.Clear();
    ubo_list.Clear();
    object_list.Clear();
    const_value_list.Clear();
}

ShaderStageIO *MaterialDescriptorManager::AddShaderStageIO(const ShaderStageBits &ssb)
{
    if(stage_io_map.KeyExist(ssb))
        return(nullptr);

    ShaderStageIO *sio=new ShaderStageIO;
            
    stage_io_map.Add(ssb, sio);

    return sio;
}

ShaderStageIO *MaterialDescriptorManager::GetShaderStageIO(const ShaderStageBits &ssb)
{
    return stage_io_map[ssb];
}

void MaterialDescriptorManager::AddUBO(ShaderStageBits bit,DescriptorSetsType type,ShaderUBOData *sd)
{
    if(bit==0||!sd)return;
            
    if(AddDescriptor(bit,type,sd))
        ubo_list.Add(sd);
}

void MaterialDescriptorManager::AddObject(ShaderStageBits bit,DescriptorSetsType type,ShaderObjectData *sd)
{
    if(bit==0||!sd)return;
            
    if(AddDescriptor(bit,type,sd))
        object_list.Add(sd);
}

void MaterialDescriptorManager::AddConstValue(ShaderConstValue *sd)
{
    if(!sd)return;

    const_value_list.Add(sd);
}

/**
* 为所有描述符产生set/binding编号
*/
void MaterialDescriptorManager::Resort()
{
    int set_index[size_t(DescriptorSetsType::RANGE_SIZE)];

    //统计并重新生成set编号
    {
        uint set_member_count[size_t(DescriptorSetsType::RANGE_SIZE)];

        hgl_zero(set_member_count);

        for(ShaderDescriptor *sd:descriptor_list)
            ++set_member_count[size_t(sd->set_type)];

        int index=0;
            
        for(uint i=0;i<size_t(DescriptorSetsType::RANGE_SIZE);++i)
        {
            if(set_member_count[i])
            {
                set_index[i]=index;
                ++index;
            }
            else
            {
                set_index[i]=-1;
            }
        }
    }
            
    //重新生成binding编号
    {
        int binding_index[size_t(DescriptorSetsType::RANGE_SIZE)];

        hgl_zero(binding_index);

        for(ShaderDescriptor *sd:descriptor_list)
        {
            sd->set=set_index[size_t(sd->set_type)];
            sd->binding=binding_index[sd->set];
            ++binding_index[sd->set];
        }
    }
}