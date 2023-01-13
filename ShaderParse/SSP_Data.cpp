#include"ShaderSectionParse.h"
#include<hgl/type/StringList.h>
#include<hgl/type/SplitString.h>
#include<hgl/type/Map.h>
#include"GLSLParse/GLSLTokenizer.h"

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

private:

    bool ParseUBO(GLSLTokenizer *parse,const UTF8String &type_name)
    {
        GLSLToken token;
        UTF8String name;
        const char *str;
        int length;

        do
        {
            str=parse->GetToken(&token, &length);

            if(token==GLSLToken::Identifier)
            {
                name.SetString(str,length);

                LOG_INFO("Find UBO: "+type_name+" "+name);
                return(true);
            }
        }while(str);

        return(false);
    }

    bool ParseData(GLSLTokenizer *parse,const UTF8String &type_name)
    {
        GLSLToken token;
        UTF8String name;
        const char *str;
        int length;

        do
        {
            str=parse->GetToken(&token, &length);

            if(token==GLSLToken::Identifier)
            {
                name.SetString(str,length);

                LOG_INFO("Find data: "+type_name+" "+name);
                return(true);
            }
        }while(str);

        return(false);
    }

    bool ParseConstValue(GLSLTokenizer *parse)
    {
        GLSLToken token;
        GLSLToken type_token=GLSLToken::None;
        UTF8String type_name;
        UTF8String name;
        UTF8String default_value;
        
        const char *str;
        int length;

        do
        {
            str=parse->GetToken(&token, &length);

            if(token>=GLSLToken::Boolean
             &&token<=GLSLToken::dvec4)
            {
                type_token=token;
                type_name.SetString(str,length);
                break;
            }
        }while(str);

        if(type_token==GLSLToken::None)
            return(false);

        do
        {
            str=parse->GetToken(&token, &length);

            if(token==GLSLToken::Identifier)
            {
                name.SetString(str, length);
                break;
            }
        }while(str);

        if(name.IsEmpty())
            return(false);

        do
        {
            str=parse->GetToken(&token, &length);

            if(token==GLSLToken::Assignment)
                break;
        }while(str);

        if(!str)
        {
            LOG_ERROR("const value need a default value.");
            return(false);
        }

        const char *start=str+length;

        do
        {
            str=parse->GetToken(&token, &length);

            if(token==GLSLToken::EndStatement)
            {
                default_value.SetString(start, str-start+length-1);
                break;
            }
        }while(str);
        
        LOG_INFO("Find const: "+type_name+" "+name+" = "+default_value);
        
        return(true);
    }

public:

    SSP_Data()
    {
        mat_section = ShaderSection::Main;
    }

    virtual ~SSP_Data() = default;

    virtual void Add(const UTF8String &line, const UTF8String &raw_line)
    {
        GLSLTokenizer parse(line.c_str(), line.Length());
        GLSLToken token;        
        int length;
        const char *str;
        UTF8String type_name;
        
        str=parse.GetToken(&token, &length);

        if(!str)return;
        
        if(token==GLSLToken::Identifier)
        {
            type_name.SetString(str,length);

            if(ParseUBO(&parse,type_name))
                return;
        }
        else if(token>=GLSLToken::sampler
              &&token<=GLSLToken::buffer)
        {
            type_name.SetString(str,length);

            if(ParseData(&parse,type_name))
                return;
        }
        else
        if(token==GLSLToken::Const)
        {
            if(ParseConstValue(&parse))
                return;
        }

        LOG_ERROR("Parse error£º"+raw_line);
    }
};//class SSP_Code:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Data(ShaderSection)
{    
    return(new SSP_Data());
}