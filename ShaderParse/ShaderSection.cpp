#include"ShaderSection.h"
#include<hgl/type/StrChar.h>

namespace
{
    constexpr const char *shader_section_name[] =
    {
        "Unknow",
        
        "Input",
        "Output",
        
        "Subpass",
        "Const",
        
        "Global",
        "PerFrame",
        "PerMaterial",
        "PerObject",
        "Instance",

        "Code",
    };
}//namespace

ShaderSection ParseShaderSection(const char *str)
{
    for(int i = 1;i < sizeof(shader_section_name) / sizeof(shader_section_name[0]);i++)
    {
        if(stricmp(str,shader_section_name[i])==0)
            return(ShaderSection(i));
    }        

    return ShaderSection::Unknow;
}