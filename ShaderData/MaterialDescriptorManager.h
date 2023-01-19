#pragma once
#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>

using namespace hgl;
using namespace vk_shader;
    
class MaterialDescriptorManager
{
    MapObject<ShaderStageBits,ShaderStageIO> stage_io_map;
        
    ObjectList<ShaderDescriptor> descriptor_list;
    List<ShaderUBOData *> ubo_list;
    List<ShaderObjectData *> object_list;
    ObjectList<ShaderConstValue> const_value_list;
    ShaderPushConstant push_constant;

private:
        
    ShaderDescriptor *GetDescriptor(const char *name);                                              ///<根据名称获取一个描述符
    bool AddDescriptor(ShaderStageBits bit,DescriptorSetsType set_type,ShaderDescriptor *new_sd);   ///<添加一个描述符，如果它本身存在，则返回false

public:

    MaterialDescriptorManager()=default;
    ~MaterialDescriptorManager()=default;

    void Clear();

    ShaderStageIO *AddShaderStageIO(const ShaderStageBits &ssb);
    ShaderStageIO *GetShaderStageIO(const ShaderStageBits &ssb);

    void AddUBO(ShaderStageBits bit,DescriptorSetsType type,ShaderUBOData *sd);
    void AddObject(ShaderStageBits bit,DescriptorSetsType type,ShaderObjectData *sd);

    void AddConstValue(ShaderConstValue *sd);

    void Resort();                                                                                  ///<为所有描述符产生set/binding编号
};//class ShaderDescriptorManager