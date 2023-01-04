#pragma once

enum class MatSection
{
    Unknow,

    PMC,
        
    VertexShader,
    VertexShaderInput,
    VertexShaderOutput,
        
    FragmentShader
};

MatSection ParseMatSection(const char *str);