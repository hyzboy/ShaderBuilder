#include"ShaderSectionParse.h"

ShaderSectionParse *CreateSSP_PMC();
ShaderSectionParse *CreateSSP_Code(ShaderSection);
ShaderSectionParse *CreateSSP_Varying(ShaderSection ms);

ShaderSectionParse *CreateSSP(const ShaderSection &ms)
{
    switch(ms)
    {
        case ShaderSection::Main:   return CreateSSP_Code(ms);
            
        case ShaderSection::Input:
        case ShaderSection::Output: return CreateSSP_Varying(ms);
            
        default:                    return nullptr;
    }
}