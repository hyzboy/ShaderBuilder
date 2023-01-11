#include"ShaderSectionParse.h"
#include<hgl/type/StringList.h>
#include<hgl/type/Map.h>

struct ShaderBindingData
{
    int sets=-1;
    int binding=-1;
};

struct UBOData:public ShaderBindingData
{
    UTF8String name;
    UTF8String type;
    UTF8String value;
};

struct ConstValue:public ShaderBindingData
{
    UTF8String type;
    UTF8String name;
};

struct PushConstant:public ShaderBindingData
{
};

class SSP_Data:public ShaderSectionParse
{
private:
    
    ShaderSection mat_section;
    
public:
    
    const ShaderSection GetSection()const override
    {
        return mat_section;
    };

public:

    SSP_Data()
    {
        mat_section = ShaderSection::Main;
    }

    virtual ~SSP_Data() = default;

    virtual void Add(const UTF8String &, const UTF8String &raw_str)
    {
    }
};//class SSP_Code:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Data(ShaderSection)
{    
    return(new SSP_Data());
}