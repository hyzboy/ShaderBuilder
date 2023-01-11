#include"ShaderSection.h"
#include<hgl/type/StrChar.h>

namespace
{
    constexpr const char *mat_section_name[] =
    {
        "Unknow",
        
        "Input",
        "Output",
        
        "Global",
        "PerFrame",
        "PerObject",

        "Main",
    };
}//namespace

ShaderSection ParseShaderSection(const char *str)
{
    for(int i = 1;i < sizeof(mat_section_name) / sizeof(mat_section_name[0]);i++)
    {
        if(stricmp(str,mat_section_name[i])==0)
            return(ShaderSection(i));
    }        

    return ShaderSection::Unknow;
}