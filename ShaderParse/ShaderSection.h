#pragma once

enum class ShaderSection
{
    Unknow,

    Input,
    Output,

    Subpass,
    Const,
    
    Global,
    PerFrame,
    PerMaterial,
    PerObject,

    Instance,
    Skeleton,

    Code,

    Attrib,
    UBO,
};

ShaderSection ParseShaderSection(const char *str);
