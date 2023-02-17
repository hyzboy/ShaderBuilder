#pragma once

#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>

class ShaderDescriptorManager;
class ShaderGen;

namespace glsl_compiler
{
    struct SPVData;
}

struct ShaderDataInfo
{
    vk_shader::ShaderStageBits shader_stage_bit;

    OSString filename;
    ShaderDescriptorManager *shader_data_manager;
    ShaderGen *sg;
    glsl_compiler::SPVData *spv;

public:

    ShaderDataInfo(const vk_shader::ShaderStageBits ssb,const OSString &fn);
    ~ShaderDataInfo();
};

using ShaderMap=ObjectMap<vk_shader::ShaderStageBits,ShaderDataInfo>;