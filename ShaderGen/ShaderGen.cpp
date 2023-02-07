#include"ShaderGen.h"
#include<hgl/filesystem/Filesystem.h>
#include<hgl/log/LogInfo.h>

ShaderGen *CreateShaderGenVertex(ShaderDataManager *sdm,const OSString &path);
ShaderGen *CreateShaderGenFragment(ShaderDataManager *sdm,const OSString &path,ShaderGen *prev);

ShaderGen *CreateShaderGen(ShaderDataManager *sdm,const OSString &path,ShaderGen *prev)
{
    if(!sdm||!filesystem::IsDirectory(path))
        return(nullptr);

    switch(sdm->GetStageBits())
    {
        case ssbVertex:     return CreateShaderGenVertex(sdm,path);
        case ssbFragment:   return CreateShaderGenFragment(sdm, path, prev);
        default:
        {
            LOG_ERROR(U8_TEXT("CreateShaderGen() unsupport stage bits: ")+sdm->GetStageName());
            return(nullptr);
        }
    }
}

ShaderGen::ShaderGen(ShaderDataManager *_sdm,const OSString &path,ShaderGen *prev)
{
    prev_gen=prev;

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

void ShaderGen::ProcSubpassInput()
{
    const auto &si_list=sdm->GetSubpassInputList();

    if(si_list.IsEmpty())return;

    for(auto si:si_list)
    {
    }        
}

void ShaderGen::ProcUBO()
{
    const auto &ubo_list=sdm->GetUBOList();

    if(ubo_list.IsEmpty())return;

    for(auto ubo:ubo_list)
        Add("layout(std140,set="+UTF8String::numberOf(ubo->set)+",binding="+UTF8String::numberOf(ubo->binding)+") uniform "+ubo->type+" "+ubo->name+";\n");

    AddLineBreak();
}

void ShaderGen::ProcObject()
{
    const auto &obj_list=sdm->GetObjectList();

    if(obj_list.IsEmpty())return;

    for(auto obj:obj_list)
        Add("layout(std140,set="+UTF8String::numberOf(obj->set)+",binding="+UTF8String::numberOf(obj->binding)+") "+obj->type+" "+obj->name+";\n");

    AddLineBreak();
}

void ShaderGen::ProcConst()
{
    const auto &cv_list=sdm->GetConstList();

    if(cv_list.IsEmpty())return;

    for(auto cv:cv_list)
        Add("layout(constant_id="+UTF8String::numberOf(cv->constant_id)+") const "+cv->type+" "+cv->name+"="+cv->value+";\n");
    
    AddLineBreak();
}

const int ShaderGen::GetStageOutputCount()const
{
    return sdm->GetShaderStageIO().output.GetCount();
}

void ShaderGen::AddStageOutput()
{
    Add("layout(location=0) out "+output_struct_name+"\tOutput;\n");
}

bool ShaderGen::Gen()
{
    Add(u8"// the Shader was created by the ShaderBuilder (" HGL_OFFICAL_WEB_U8 u8")\n"
        u8"\n"
        u8"#version 460 core\n\n");

    PreProcess();
    
    ProcInclude();

    ProcSubpassInput();
    ProcUBO();
    ProcObject();
    ProcConst();
    
    if(!Begin())
        return(false);

    Add(sdm->GetSources());

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
