#include"ShaderSectionParse.h"
#include<hgl/type/StringList.h>

class SSP_Code:public ShaderSectionParse
{
private:
    
    ShaderSection shader_section;
    UTF8StringList code_list;

public:
    
    const ShaderSection GetSection()const override
    {
        return shader_section;
    };

public:

    SSP_Code()
    {
        shader_section = ShaderSection::Code;
    }

    virtual ~SSP_Code() = default;

    virtual void Add(const UTF8String &, const UTF8String &raw_str)
    {
        code_list.Add(raw_str);
    }
};//class SSP_Code:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Code(ShaderSection)
{    
    return(new SSP_Code());
}