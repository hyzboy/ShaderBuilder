#pragma once

#include"vulkan/VKShaderCommon.h"
#include<vulkan/vulkan.h>
#include<hgl/type/Map.h>

class ShaderDescriptorManager;
class ShaderGen;

namespace glsl_compiler
{
    struct SPVData;
}

struct ShaderDataInfo
{
    VkShaderStageFlagBits shader_stage_bit;

    OSString filename;
    ShaderDescriptorManager *sdm;
    ShaderGen *sg;
    glsl_compiler::SPVData *spv;

public:

    ShaderDataInfo(const VkShaderStageFlagBits ssb,const OSString &fn);
    ~ShaderDataInfo();
};

using ShaderMap=ObjectMap<VkShaderStageFlagBits,ShaderDataInfo>;