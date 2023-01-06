#include"MatSectionParse.h"

MatSectionParse *CreateMSP_PMC();
MatSectionParse *CreateMSP_Code(MatSection);

MatSectionParse *CreateMSP(const MatSection &ms)
{
    switch(ms)
    {
        case MatSection::PMC:
            return CreateMSP_PMC();
        case MatSection::VertexShader:
        case MatSection::FragmentShader:
            return CreateMSP_Code(ms);
        default:
            return nullptr;
    }
}