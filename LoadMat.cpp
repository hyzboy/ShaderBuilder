#include<hgl/type/LoadStringList.h>
#include<hgl/type/Map.h>
#include<hgl/type/SplitString.h>
#include<hgl/filesystem/FileSystem.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"vulkan/VKShaderCommon.h"
#include"ShaderParse/ShaderSection.h"
#include"ShaderParse/ShaderSectionParse.h"

using namespace hgl;

namespace
{
    bool LoadShader(vk_shader::ShaderStageBits ssb,const OSString &filename)
    {
        UTF8StringList sl;

        if(!LoadStringListFromTextFile(sl,filename))
        {
            LOG_ERROR(OS_TEXT("Load shader file failure. filename: ")+filename);
            return(false);
        }

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

                ssp=CreateSSP(shader_section);
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
        return(true);
    }
}//namespace

bool LoadMat(const OSString &filename)
{
    UTF8StringList sl;

    if(!LoadStringListFromTextFile(sl,filename))
    {
        LOG_ERROR(OS_TEXT("Load .Mat file failure. filename: ")+filename);
        return(false);
    }

    UTF8String pmc;
    Map<vk_shader::ShaderType,OSString> shaderfile;

    UTF8String left,right;
    OSString path;

    path=filesystem::ClipPathname(filename,false);

    for(int i=0;i<sl.GetCount();i++)
    {
        SpliteByString(sl[i],UTF8String(u8":"),&left,&right);

        left=left.Trim();
        right=right.Trim();

        if(left.IsEmpty()||right.IsEmpty())
            continue;

        if(left.CaseComp(u8"pmc")==0)
            pmc=left;
        else
        {
            vk_shader::ShaderType type=glsl_compiler::GetType(left);

            if(type==0)
                continue;
            
            OSString fullname=filesystem::MergeFilename(path,ToOSString(right+U8_TEXT(".")+left));

            if(!filesystem::FileExist(fullname))
            {
                LOG_ERROR(OS_TEXT("Shader file not exist. filename: ")+fullname);
                return(false);
            }
            
            shaderfile.Add(type,fullname);
        }
    }

    OSString shader_filename;
    
    if(!shaderfile.Get(vk_shader::ssbFragment,shader_filename))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);
    }
    else
    {
        if(!LoadShader(vk_shader::ssbFragment,shader_filename))
            return(false);
    }
    
    if(!shaderfile.Get(vk_shader::ssbVertex,shader_filename))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);        
    }
    else
    {
        if(!LoadShader(vk_shader::ssbVertex, shader_filename))
            return(false);
    }

    return(true);
}