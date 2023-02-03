#include<hgl/type/LoadStringList.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"ShaderParse/ShaderSection.h"
#include"ShaderParse/ShaderSectionParse.h"
#include"ShaderData/ShaderDataManager.h"

ShaderDataManager *LoadShader(vk_shader::ShaderStageBits ssb,const OSString &filename)
{
    UTF8StringList sl;

    if(!LoadStringListFromTextFile(sl,filename))
    {
        LOG_ERROR(OS_TEXT("Load shader file failure. filename: ")+filename);
        return(nullptr);
    }

    ShaderDataManager *sdm=new ShaderDataManager(ssb);
        
    const UTF8String ssb_name=GetShaderStageName(ssb);

    LOG_INFO("Start parsing the "+ssb_name+" Shader............");

    ShaderSectionParse *ssp=nullptr;

    for(int i=0;i<sl.GetCount();i++)
    {
        UTF8String raw_line=sl[i];
        UTF8String line;

        if(raw_line.IsEmpty())continue;

        //清除单行注释
        {
            int comment_pos;
            comment_pos=raw_line.FindString("//");

            if(comment_pos!=-1)
            {
                line=raw_line;
                line.ClipLeft(comment_pos);
            }
            else
            {
                line=raw_line;
            }
        }

        line=line.Trim();        //清除左右两端不可视字符

        if(line.IsEmpty())continue;

        if(line.GetFirstChar()=='['
          &&line.GetLastChar()==']')
        {
            line=line.SubString(1, line.Length()-2);

            LOG_INFO("section: "+line);

            ShaderSection shader_section=ParseShaderSection(line);

            SAFE_CLEAR(ssp);

            ssp=CreateSSP(shader_section,sdm);
        }
        else
        {
            if(ssp)
            {
                ssp->Add(line,raw_line);
            }
            else
            {
                LOG_INFO(line);
            }
        }
    }

    SAFE_CLEAR(ssp);

    LOG_INFO("End parsing the "+ssb_name+" Shader............");
    return(sdm);
}
