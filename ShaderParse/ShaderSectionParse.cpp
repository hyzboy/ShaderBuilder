#include"ShaderSectionParse.h"

ShaderSectionParse *CreateSSP_Code(ShaderSection);
ShaderSectionParse *CreateSSP_Varying(ShaderSection ms);
ShaderSectionParse *CreateSSP_Const(ShaderSection);
ShaderSectionParse *CreateSSP_Data(ShaderSection);

ShaderSectionParse *CreateSSP(const ShaderSection &ms)
{
    switch(ms)
    {
        case ShaderSection::Code:       return CreateSSP_Code(ms);
            
        case ShaderSection::Input:
        case ShaderSection::Output:     return CreateSSP_Varying(ms);

        case ShaderSection::Const:      return CreateSSP_Const(ms);

        case ShaderSection::Global:
        case ShaderSection::PerFrame:
        case ShaderSection::PerObject:
        case ShaderSection::Instance:   return CreateSSP_Data(ms);
            
        default:                        return nullptr;
    }
}