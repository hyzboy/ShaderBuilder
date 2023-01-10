#pragma once

enum class ShaderSection
{
    Unknow,

    Input,
    Output,
    UBO,

    Main
};

ShaderSection ParseShaderSection(const char *str);
