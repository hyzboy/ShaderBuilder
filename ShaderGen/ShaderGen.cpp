#include"ShaderGen.h"
#include<hgl/filesystem/Filesystem.h>
#include<hgl/log/LogInfo.h>

ShaderGen *CreateShaderGenVertex(ShaderDataManager *sdm,const OSString &path);

ShaderGen *CreateShaderGen(ShaderDataManager *sdm,const OSString &path)
{
    if(!sdm||!filesystem::IsDirectory(path))
        return(nullptr);

    if(sdm->GetStageBits()==ssbVertex)
        return CreateShaderGenVertex(sdm,path);
    else
    {
        LOG_ERROR(U8_TEXT("CreateShaderGen() unsupport stage bits: ")+sdm->GetStageName());
        return(nullptr);
    }
}

ShaderGen::ShaderGen(ShaderDataManager *_sdm,const OSString &path)
{
    sdm=_sdm;
    
    output_path=path;
}

void ShaderGen::Add(const UTF8String &str)
{
    if(str.IsEmpty())return;
    
    shader_source.Strcat(str);
}

void ShaderGen::AddLineBreak()
{
    shader_source.Strcat("\n");
}

void ShaderGen::ProcInclude()
{
    if(include_list.IsEmpty())return;
    
    for(auto fn:include_list)
        Add("#include\""+(*fn)+"\"\n");

    AddLineBreak();
}

void ShaderGen::AddStageOutput()
{
    Add("layout(location=0) out "+output_struct_name+"\tOutput;\n");
}

bool ShaderGen::Gen()
{
    Add(u8"// the Shader was created by the ShaderBuilder (" HGL_OFFICAL_WEB_U8 u8")\n"
        u8"\n"
        u8"#version 460\n\n");

    PreProcess();
    
    ProcInclude();
    
    if(!Begin())
        return(false);

    if(!End())
        return(false);

    {
        UTF8String fn=sdm->GetStageName()+UTF8String(".glsl");

        OSString ofn=filesystem::MergeFilename(output_path, ToOSString(fn));

        if(!filesystem::SaveMemoryToFile(ofn, shader_source.c_str(), shader_source.Length()))
        {
            LOG_ERROR(OS_TEXT("Save file '")+ofn+OS_TEXT("' failed."));
            return(false);
        }
        else
        {
            LOG_INFO(OS_TEXT("Save file '")+ofn+OS_TEXT("' success."));
        }
    }

    return(true);
}