#include"MatSectionParse.h"
#include<hgl/type/StringList.h>

class MSP_Code:public MatSectionParse
{
private:
    
    MatSection mat_section;
    UTF8StringList code_list;

public:
    
    const MatSection GetSection()const override
    {
        return mat_section;
    };

public:

    MSP_Code(MatSection ms)
    {
        mat_section = ms;
    }

    virtual ~MSP_Code() = default;

    virtual void Add(const UTF8String &, const UTF8String &raw_str)
    {
        code_list.Add(raw_str);
    }
};//class MSP_Code:public MatSectionParse

MatSectionParse *CreateMSP_Code(MatSection ms)
{
    if(ms!=MatSection::VertexShader
     &&ms!=MatSection::FragmentShader)
        return(nullptr);
    
    return(new MSP_Code(ms));
}