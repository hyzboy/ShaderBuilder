#include"MatSectionParse.h"
#include<hgl/type/StringList.h>

class MSP_PMC:public MatSectionParse
{
    UTF8StringList pmc_list;

public:

    const MatSection GetSection()const override
    {
        return MatSection::PMC;
    }

public:

    void Add(const UTF8String &str,const UTF8String &)
    {
        if(str.IsEmpty())return;

        if(!pmc_list.CaseExist(str))
            pmc_list.Add(str);
    }
};//class MSP_PMC

MatSectionParse *CreateMSP_PMC()
{
    return(new MSP_PMC);
}