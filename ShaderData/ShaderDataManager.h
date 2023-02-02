#pragma once

#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>
#include<hgl/type/StringList.h>

using namespace hgl;
using namespace vk_shader;

/**
* Shader数据管理器,用于生成正式Shader前的资源统计
*/
class ShaderDataManager
{
    ShaderStageBits ssb;

    ShaderStageIO stage_io;
    
    AnsiStringList                  descriptor_name_list;
    ObjectList<ShaderDescriptor>    descriptor_list;
    
    List<ShaderUBOData *>           ubo_list;
    List<ShaderObjectData *>        object_list;
    
    ObjectList<ShaderConstValue>    const_value_list;
    ObjectList<ShaderSubpassInput>  subpass_input;
    
    ShaderPushConstant              push_constant;

private:

    bool AddDescriptor(DescriptorSetsType set_type,ShaderDescriptor *new_sd);                       ///<添加一个描述符，如果它本身存在，则返回false

public:

    ShaderDataManager(ShaderStageBits,ShaderStageBits,ShaderStageBits);
    ~ShaderDataManager(){Clear();}

    void Clear();

public:

    bool AddInput(ShaderStage *);
    bool AddOutput(ShaderStage *);

    bool AddUBO(DescriptorSetsType type,ShaderUBOData *sd);
    bool AddObject(DescriptorSetsType type,ShaderObjectData *sd);

    bool AddConstValue(ShaderConstValue *sd);    
    bool AddSubpassInput(const UTF8String name,uint8_t index);
    
    void SetPushConstant(const UTF8String name,uint8_t offset,uint8_t size);
};//class ShaderDataManager
