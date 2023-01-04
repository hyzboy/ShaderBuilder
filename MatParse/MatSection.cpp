#include"MatSection.h"
#include<hgl/type/StrChar.h>

namespace
{
    constexpr const char *mat_section_name[] =
    {
        "Unknow",
        
        "PMC",

        "Vertex Shader",
        "Vertex Shader/Input",
        "Vertex Shader/Output",

        "Fragment Shader",        
    };
}//namespace

MatSection ParseMatSection(const char *str)
{
    for(int i = 1;i < sizeof(mat_section_name) / sizeof(mat_section_name[0]);i++)
    {
        if(stricmp(str,mat_section_name[i])==0)
            return(MatSection(i));
    }        

    return MatSection::Unknow;
}