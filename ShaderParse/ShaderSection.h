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

    Code
};

ShaderSection ParseShaderSection(const char *str);
