#include<hgl/type/LoadStringList.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"ShaderParse/ShaderSection.h"
#include"ShaderParse/ShaderSectionParse.h"
#include"ShaderData/ShaderDescriptorManager.h"
#include"ShaderData/MaterialDataInfo.h"

bool LoadShader(ShaderDescriptorManager *sdm,const UTF8StringList &source_codes)
{
    const UTF8String ssb_name=sdm->GetStageName();

    LOG_INFO("Start parsing the "+ssb_name+" Shader............");

    ShaderSectionParse *ssp=nullptr;

    for(int i=0;i<source_codes.GetCount();i++)
    {
        UTF8String raw_line=source_codes[i];
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
    return(true);
}

bool LoadShader(MaterialDataInfo *cvd)
{
    if(!cvd)return(false);

    if(!cvd->shader_map.KeyExist(VK_SHADER_STAGE_FRAGMENT_BIT))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);
    }

    if(!cvd->shader_map.KeyExist(VK_SHADER_STAGE_VERTEX_BIT))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);
    }

    {
        auto *dp=cvd->shader_map.GetDataList();
        
        for(int i=0;i<cvd->shader_map.GetCount();i++)
        {   
            UTF8StringList codes;
            ShaderDataInfo *sdi=(*dp)->right;

            if(!LoadStringListFromTextFile(codes,sdi->filename))
            {
                LOG_ERROR(OS_TEXT("Load shader file failure. filename: ")+sdi->filename);
                continue;
            }
            
            ShaderDescriptorManager *sdm=new ShaderDescriptorManager((*dp)->left, &cvd->mdm);

            if(!LoadShader(sdm, codes))
            {
                delete sdm;
                return(false);
            }

            sdi->sdm=sdm;

            ++dp;
        }
    }

    return(true);
}