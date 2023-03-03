#include"ShaderSectionParse.h"
#include<hgl/type/StringList.h>
#include<hgl/type/SplitString.h>
#include<hgl/type/Map.h>
#include"GLSLParse/GLSLTokenizer.h"
#include"vulkan/VKShaderCommon.h"
#include"ShaderData/ShaderDescriptorManager.h"
#include<hgl/graph/VKDescriptorSetType.h>

using namespace vk_shader;
using namespace hgl::graph;

class SSP_Data:public ShaderSectionParse
{
private:
    
    ShaderSection shader_section;

    DescriptorSetType desc_set_type;
    
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

                UBODescriptor *ubo=new UBODescriptor;

                ubo->type=type_name;
                ubo->name=name;

                sdm->AddUBO(desc_set_type,ubo);

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

                ShaderObjectData *ubo=new ShaderObjectData;

                ubo->type=type_name;
                ubo->name=name;

                sdm->AddObject(desc_set_type,ubo);
                
                return(true);
            }
        }while(str);

        return(false);
    }

public:

    SSP_Data(ShaderSection ss,ShaderDescriptorManager *s):ShaderSectionParse(s)
    {
        shader_section = ss;

        if(ss==ShaderSection::PerFrame)     desc_set_type=DescriptorSetType::PerFrame;     else
        if(ss==ShaderSection::PerMaterial)  desc_set_type=DescriptorSetType::PerMaterialInstance;  else
        if(ss==ShaderSection::PerObject)    desc_set_type=DescriptorSetType::PerObject;    else
        if(ss==ShaderSection::Instance)     desc_set_type=DescriptorSetType::Instance;     else
        if(ss==ShaderSection::Skeleton)     desc_set_type=DescriptorSetType::Skeleton;     else
            desc_set_type=DescriptorSetType::Global;
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

        LOG_ERROR("Parse error: "+raw_line);
    }
};//class SSP_Code:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Data(ShaderSection ss,ShaderDescriptorManager *sdm)
{    
    return(new SSP_Data(ss,sdm));
}
