#include"ShaderSectionParse.h"

ShaderSectionParse *CreateSSP_Code      (ShaderSection,ShaderDescriptorManager *);
ShaderSectionParse *CreateSSP_Varying   (ShaderSection,ShaderDescriptorManager *);
ShaderSectionParse *CreateSSP_Const     (ShaderSection,ShaderDescriptorManager *);
ShaderSectionParse *CreateSSP_Data      (ShaderSection,ShaderDescriptorManager *);

ShaderSectionParse *CreateSSP(const ShaderSection &ms,ShaderDescriptorManager *sdm)
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

        case ShaderSection::Instance:   
        case ShaderSection::Skeleton:   return CreateSSP_Data(ms,sdm);

        default:                        return nullptr;
    }
}