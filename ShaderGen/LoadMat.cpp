#include<hgl/type/LoadStringList.h>
#include<hgl/type/Map.h>
#include<hgl/type/SplitString.h>
#include<hgl/filesystem/FileSystem.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"vulkan/VKShaderCommon.h"
#include"ShaderData/ShaderDataManager.h"

using namespace hgl;

ShaderDataManager *LoadShader(vk_shader::ShaderStageBits ssb,const OSString &filename);

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

    ObjectList<ShaderDataManager> SDMList;
    
    if(!shaderfile.Get(vk_shader::ssbFragment,shader_filename))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);
    }
    else
    {
        ShaderDataManager *sdm=LoadShader(vk_shader::ssbFragment,shader_filename);

        if(!sdm)
            return(false);

        SDMList.Add(sdm);
    }
    
    if(!shaderfile.Get(vk_shader::ssbVertex,shader_filename))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);        
    }
    else
    {
        ShaderDataManager *sdm=LoadShader(vk_shader::ssbVertex, shader_filename);

        if(!sdm)
            return(false);

        SDMList.Add(sdm);
    }

    ResortShaderDescriptor(SDMList);

    return(true);
}