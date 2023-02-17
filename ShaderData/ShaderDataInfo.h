#pragma once

#include"vulkan/VKShaderCommon.h"

class ShaderDataManager;
class ShaderGen;

namespace glsl_compiler
{
    struct SPVData;
}

struct ShaderDataInfo
{
    vk_shader::ShaderStageBits shader_stage_bit;

    OSString filename;
    ShaderDataManager *shader_data_manager;
    ShaderGen *sg;
    glsl_compiler::SPVData *spv;

public:

    ShaderDataInfo(const vk_shader::ShaderStageBits ssb,const OSString &fn);
    ~ShaderDataInfo();
};