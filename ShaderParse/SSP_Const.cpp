#include"ShaderSectionParse.h"
#include<hgl/type/StringList.h>
#include<hgl/type/SplitString.h>
#include<hgl/type/Map.h>
#include"GLSLParse/GLSLTokenizer.h"
#include"vulkan/VKShaderCommon.h"
#include"ShaderData/ShaderDescriptorManager.h"

using namespace vk_shader;

class SSP_Const:public ShaderSectionParse
{
public:
    
    const ShaderSection GetSection()const override
    {
        return ShaderSection::Const;
    };

private:

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

        ShaderConstValue *cv=new ShaderConstValue;

        cv->type=type_name;
        cv->name=name;
        cv->value=default_value;

        if(!sdm->AddConstValue(cv))
            delete cv;

        return(true);
    }

public:

    SSP_Const(ShaderDescriptorManager *s):ShaderSectionParse(s){}
    virtual ~SSP_Const() = default;

    virtual void Add(const UTF8String &line, const UTF8String &raw_line)
    {
        GLSLTokenizer parse(line.c_str(), line.Length());
        
        if(ParseConstValue(&parse))
            return;

        LOG_ERROR("Parse error: "+raw_line);
    }
};//class SSP_Const:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Const(ShaderSection,ShaderDescriptorManager *sdm)
{    
    return(new SSP_Const(sdm));
}
