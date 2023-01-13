#include"ShaderSectionParse.h"

ShaderSectionParse *CreateSSP_PMC();
ShaderSectionParse *CreateSSP_Code(ShaderSection);
ShaderSectionParse *CreateSSP_Varying(ShaderSection ms);
ShaderSectionParse *CreateSSP_Data(ShaderSection);

ShaderSectionParse *CreateSSP(const ShaderSection &ms)
{
    switch(ms)
    {
        case ShaderSection::Code:       return CreateSSP_Code(ms);
            
        case ShaderSection::Input:
        case ShaderSection::Output:     return CreateSSP_Varying(ms);

        case ShaderSection::Global:
        case ShaderSection::PerFrame:
        case ShaderSection::PerObject:  return CreateSSP_Data(ms);
            
        default:                        return nullptr;
    }
}