#pragma once

#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>

using namespace hgl;
using namespace vk_shader;

struct ShaderDescriptorSet
{
    DescriptorSetsType set_type;

    int set;
    int count;

    ObjectMap<AnsiString,ShaderDescriptor>  descriptor_map;
    
    List<ShaderUBOData *>                   ubo_list;
    List<ShaderObjectData *>                object_list;

public:    

    const ShaderDescriptor *AddDescriptor(ShaderStageBits ssb,ShaderDescriptor *new_sd);                       ///<���һ���������������������ڣ��򷵻�false
};

typedef ShaderDescriptorSet ShaderDescriptorSetArray[size_t(DescriptorSetsType::RANGE_SIZE)];
    
class MaterialDescriptorManager
{
    ShaderDescriptorSetArray desc_set_array;

private:

public:
    
    MaterialDescriptorManager();
    ~MaterialDescriptorManager()=default;

    const ShaderUBOData *AddUBO(ShaderStageBits ssb,DescriptorSetsType type,ShaderUBOData *sd);
    const ShaderObjectData *AddObject(ShaderStageBits ssb,DescriptorSetsType type,ShaderObjectData *sd);

    void Resort();      //�������set����binding��
};