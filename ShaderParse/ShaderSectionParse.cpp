#include"ShaderSectionParse.h"

ShaderSectionParse *CreateSSP_Code      (ShaderSection,ShaderDataManager *);
ShaderSectionParse *CreateSSP_Varying   (ShaderSection,ShaderDataManager *);
ShaderSectionParse *CreateSSP_Const     (ShaderSection,ShaderDataManager *);
ShaderSectionParse *CreateSSP_Data      (ShaderSection,ShaderDataManager *);

ShaderSectionParse *CreateSSP(const ShaderSection &ms,ShaderDataManager *sdm)
{
    switch(ms)
    {
        case ShaderSection::Code:       return CreateSSP_Code(ms,sdm);
            
        case ShaderSection::Input:
        case ShaderSection::Output:     return CreateSSP_Varying(ms,sdm);

        case ShaderSection::Const:      return CreateSSP_Const(ms,sdm);

        case ShaderSection::Global:
        case ShaderSection::PerFrame:
        case ShaderSection::PerMaterial:
        case ShaderSection::PerObject:
        case ShaderSection::Instance:   return CreateSSP_Data(ms,sdm);
            
        default:                        return nullptr;
    }
}