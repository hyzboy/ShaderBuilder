#include"ShaderSectionParse.h"
#include"MatData/Varying.h"
#include"GLSLParse/GLSLTokenizer.h"
#include<hgl/type/StringList.h>

class SSP_Varying:public ShaderSectionParse
{
private:
    
    ShaderSection shader_section;
    
    UTF8StringList varying_names;
    VaryingList varying_list;
    
public:
    
    const ShaderSection GetSection()const override
    {
        return shader_section;
    };

private:
    
    Varying *ParseVarying(const UTF8String &str)
    {
        GLSLTokenizer parse(str.c_str(),str.Length());
            
        Varying *v=new Varying;

        GLSLToken token;
        int length;

        const char *p=parse.GetToken(&token,&length);

        if(!p)
            return(nullptr);

        if(token==GLSLToken::Flat
         ||token==GLSLToken::Noperspective
         ||token==GLSLToken::Smooth)
            v->interpolation.SetString(p,length);

        do
        {
            if(token>GLSLToken::Void
            &&token<=GLSLToken::dvec4)
            {
                v->type.SetString(p,length);
                break;
            }

            p=parse.GetToken(&token,&length);
        }while(p);

        do
        {
            p=parse.GetToken(&token,&length);
            if(p&&token==GLSLToken::Identifier)
            {
                v->name.SetString(p,length);
                break;
            }
        }while(p);

        do
        {
            p=parse.GetToken(&token,&length);
            if(p&&token==GLSLToken::OnelineComment)
            {
                v->comment.SetString(p+2,length-2);
                break;
            }
        }while(p);

        if(!v->name.IsEmpty()
         &&!v->type.IsEmpty())
        {
        #ifdef _DEBUG
            UTF8String info="  Varying: ";
                
            if(!v->interpolation.IsEmpty())
            {
                info+=v->interpolation;
                info+=" ";
            }

            info+=v->type;
            info+=" ";
            info+=v->name;

            if(!v->comment.IsEmpty())
            {
                info+=";    //";
                info+=v->comment;
            }

            LOG_INFO(info.c_str());
        #endif//_DEBUG

            return v;
        }

        delete v;
        return(nullptr);
    }

public:

    SSP_Varying(const ShaderSection &ms)
    {
        shader_section = ms;
    }

    ~SSP_Varying()=default;
    
    virtual void Add(const UTF8String &, const UTF8String &raw_str)
    {
        Varying *v=ParseVarying(raw_str);

        if(!v)
        {
            LOG_ERROR(U8_TEXT("Varying parse failed: \"")+raw_str+U8_TEXT("\""));
            return;
        }

        if(varying_names.CaseExist(v->name))
        {
            LOG_ERROR(U8_TEXT("Varying name repeat: \"")+raw_str+U8_TEXT("\""));
            delete v;
        }
        
        varying_list.Add(v);
        varying_names.Add(v->name);
    }
};//class SSP_Varying:public ShaderSectionParse

ShaderSectionParse *CreateSSP_Varying(ShaderSection ms)
{
    if(ms!=ShaderSection::Input
     &&ms!=ShaderSection::Output)
        return(nullptr);
    
    return(new SSP_Varying(ms));
}