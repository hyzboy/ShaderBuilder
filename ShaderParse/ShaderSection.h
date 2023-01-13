#pragma once

enum class ShaderSection
{
    Unknow,

    Input,
    Output,
    
    Global,
    PerFrame,
    PerObject,

    Code
};

ShaderSection ParseShaderSection(const char *str);
