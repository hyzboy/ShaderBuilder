#include"MatSectionParse.h"

MatSectionParse *CreateMSP_PMC();

MatSectionParse *CreateMSP(const MatSection &ms)
{
    switch(ms)
    {
        case MatSection::PMC:
            return CreateMSP_PMC();
        default:
            return nullptr;
    }
}