#include<hgl/log/LogInfo.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/Endian.h>
#include"GLSLCompiler/GLSLCompiler.h"

using namespace hgl;
using namespace hgl::filesystem;

bool LoadConfig();

const UTF8String GetCurWorkPath();
const UTF8String GetShaderLibraryPath();

bool ConvertMaterial(const OSString &filename,const OSString &output_path);

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
    OSString output_path;
    OSString ext_name=OS_TEXT(".material");
    
    if(argc<3)  output_path=ReplaceExtName(input_filename,ext_name);
        else    output_path=argv[2];

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

    ConvertMaterial(input_filename,output_path);

    glsl_compiler::Close();
    return 0;
}
