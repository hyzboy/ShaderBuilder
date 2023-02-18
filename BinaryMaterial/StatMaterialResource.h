#pragma once

#include"GLSLCompiler/GLSLCompiler.h"

struct MaterialDescriptorResource
{
    glsl_compiler::Descriptor *desc;
    uint32_t shader_stage_bits;
    uint32_t desc_type;

public:

    const int Comp(const MaterialDescriptorResource &msr)const;
};

using MDRList=ObjectMap<AnsiString,MaterialDescriptorResource>;