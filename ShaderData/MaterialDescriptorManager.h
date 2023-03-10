#pragma once

#include<hgl/graph/VKShaderDescriptor.h>
#include<hgl/graph/VKDescriptorSetType.h>
#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>

using namespace hgl;
using namespace hgl::graph;
using namespace vk_shader;

/**
* 材质描述符管理器</p>
* 该类使用于SHADER生成前，用于统计编号set/binding
*/
class MaterialDescriptorManager
{
    struct ShaderDescriptorSet
    {
        DescriptorSetType set_type;

        int set;
        int count;

        ObjectMap<UTF8String,ShaderDescriptor>  descriptor_map;

        List<UBODescriptor *>                   ubo_list;
        List<ShaderObjectData *>                object_list;

    public:

        const ShaderDescriptor *AddDescriptor(VkShaderStageFlagBits ssb,ShaderDescriptor *new_sd);                       ///<添加一个描述符，如果它本身存在，则返回false
    };

    using ShaderDescriptorSetArray=ShaderDescriptorSet[size_t(DescriptorSetType::RANGE_SIZE)];

    ShaderDescriptorSetArray desc_set_array;

private:

public:
    
    MaterialDescriptorManager();
    ~MaterialDescriptorManager()=default;

    const UBODescriptor *AddUBO(VkShaderStageFlagBits ssb,DescriptorSetType type,UBODescriptor *sd);
    const ShaderObjectData *AddObject(VkShaderStageFlagBits ssb,DescriptorSetType type,ShaderObjectData *sd);

    const DescriptorSetType GetSetType(const AnsiString &)const;

    void Resort();      //排序产生set号与binding号
};
