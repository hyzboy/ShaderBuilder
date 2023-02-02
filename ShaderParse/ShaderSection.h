#pragma once

enum class ShaderSection
{
    Unknow,

    Input,
    Output,

    Const,
    
    Global,
    PerFrame,
    PerObject,
    Instance,

    Code
};

ShaderSection ParseShaderSection(const char *str);
