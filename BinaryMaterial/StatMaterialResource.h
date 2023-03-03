#pragma once

#include"GLSLCompiler/GLSLCompiler.h"
#include"vulkan/VKShaderCommon.h"
#include<hgl/graph/VKDescriptorSetType.h>

using namespace hgl::graph;

struct MaterialDescriptorResource
{
    glsl_compiler::Descriptor *desc;
    uint32_t shader_stage_bits;
    uint32_t desc_type;

    DescriptorSetType set_type;

public:

    const int Comp(const MaterialDescriptorResource &msr)const;
};

using MDRList=ObjectMap<AnsiString,MaterialDescriptorResource>;