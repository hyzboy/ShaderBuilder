#include<hgl/log/LogInfo.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/Endian.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"ShaderData/MaterialDataInfo.h"

using namespace hgl;
using namespace hgl::filesystem;

bool LoadConfig();

const UTF8String GetCurWorkPath();
const UTF8String GetShaderLibraryPath();

bool LoadMat(MaterialDataInfo *);
bool LoadShader(MaterialDataInfo *);

void ResortShader(ShaderMap &);
bool CompileToSPV(MaterialDataInfo &);

bool MakeBinaryMaterial(const OSString &,ShaderMap &,const MaterialDescriptorManager *);

int os_main(int argc,os_char **argv)
{
    LOG_INFO(OS_TEXT("ShaderBuilder v1.0\nCopyright 2022 www.hyzgame.com\n\n"));

    InitLogger(OS_TEXT("ShaderBuilder"));

    if(!LoadConfig())
        return(-1);

    if(argc<=1)
    {
        LOG_INFO(OS_TEXT("\nUsage: ShaderBuilder <input material source file> <output path>\n"));
        return(-1);
    }

    OSString input_filename = argv[1];
    OSString output_path = argv[2];
    OSString ext_name=OS_TEXT(".material");

    LOG_INFO(OS_TEXT("Input File: ") + input_filename);
    LOG_INFO(OS_TEXT("Output Path: ") + output_path);

    if(!filesystem::IsDirectory(output_path))
    {
        if(!filesystem::MakePath(output_path))
        {
            LOG_ERROR(OS_TEXT("Can't create output path."));
            return(-1);
        }
        else
        {
            LOG_INFO(OS_TEXT("Create output path OK."));
        }
    }

    glsl_compiler::Init();

    {
        const UTF8String output_u8path=to_u8(output_path);

        glsl_compiler::AddGLSLIncludePath(output_u8path.c_str());
        glsl_compiler::AddGLSLIncludePath(GetCurWorkPath().c_str());
        glsl_compiler::AddGLSLIncludePath(GetShaderLibraryPath().c_str());
        glsl_compiler::RebuildGLSLIncludePath();
    }

    MaterialDataInfo mdi;

    mdi.filename=input_filename;
    mdi.output_path=output_path;

    {
        if(!LoadMat(&mdi))
            return(false);

        if(!LoadShader(&mdi))
            return(false);

        mdi.mdm.Resort();

        ResortShader(mdi.shader_map);
    }

    {
        if(CompileToSPV(mdi))
        {
            OSString main_fn=filesystem::ClipFileMainname(input_filename);
            OSString out_fn=MergeFilename(output_path,main_fn+ext_name);

            MakeBinaryMaterial(out_fn,mdi.shader_map,&mdi.mdm);
        }
    }

    glsl_compiler::Close();
    return 0;
}
