#pragma once

#include"GLSLCompiler/GLSLCompiler.h"
#include"vulkan/VKShaderCommon.h"

struct MaterialDescriptorResource
{
    glsl_compiler::Descriptor *desc;
    uint32_t shader_stage_bits;
    uint32_t desc_type;

    vk_shader::DescriptorSetType set_type;

public:

    const int Comp(const MaterialDescriptorResource &msr)const;
};

using MDRList=ObjectMap<AnsiString,MaterialDescriptorResource>;