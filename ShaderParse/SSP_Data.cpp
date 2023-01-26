#include"ShaderSectionParse.h"
#include<hgl/type/StringList.h>
#include<hgl/type/SplitString.h>
#include<hgl/type/Map.h>
#include"GLSLParse/GLSLTokenizer.h"
#include"vulkan/VKShaderCommon.h"

using namespace vk_shader;

class SSP_Data:public ShaderSectionParse
{
private:
    
    ShaderSection shader_section;

    ObjectList<ShaderUBOData> ubo_list;
    ObjectList<ShaderUBOData> data_list;
    ObjectList<ShaderConstValue> const_value_list;
    
public:
    
    const ShaderSection GetSection()const override
    {
        return shader_section;
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

                UBOData *ubo=new UBOData;

                ubo->type=type_name;
                ubo->name=name;

                ubo_list.Add(ubo);

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

                UBOData *ubo=new UBOData;

                ubo->type=type_name;
                ubo->name=name;

                data_list.Add(ubo);
                
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

        ConstValue *cv=new ConstValue;

        cv->type=type_name;
        cv->name=name;
        cv->value=default_value;

        return(true);
    }

public:

    SSP_Data(ShaderSection ss)
    {
        shader_section = ss;
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

    const ObjectList<UBOData> *GetUBOList()const { return &ubo_list; }
    const ObjectList<UBOData> *GetDataList()const { return &data_list; }
    const ObjectList<ConstValue> *GetConstValueList()const { return &const_value_list; }
};//class SSP_Code:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Data(ShaderSection ss)
{    
    return(new SSP_Data(ss));
}