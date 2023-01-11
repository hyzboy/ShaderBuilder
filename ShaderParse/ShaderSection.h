#pragma once

enum class ShaderSection
{
    Unknow,

    Input,
    Output,
    
    Global,
    PerFrame,
    PerObject,

    Main
};

ShaderSection ParseShaderSection(const char *str);
