#include<hgl/log/LogInfo.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/Endian.h>
#include"GLSLCompiler/GLSLCompiler.h"

using namespace hgl;
using namespace hgl::filesystem;

bool SetShaderLibraryPath(const OSString &);
bool LoadConfig();
void SaveConfig();

bool LoadMat(const OSString &filename);

int os_main(int argc,os_char **argv)
{
    LOG_INFO(OS_TEXT("ShaderBuilder v1.0\nCopyright 2022 www.hyzgame.com\n\n"));

    InitLogger(OS_TEXT("ShaderBuilder"));

    LoadConfig();

    if(argc<=1)
    {
        LOG_INFO(OS_TEXT("\nUsage: ShaderBuilder <input material source file> <output material binary file>\n"
                           "       ShaderBuilder /C <Shader Library Path>"));
        return(-1);
    }

    {
        util::CmdParse cp(argc, argv);

        OSString path;
        
        if(cp.GetString(OS_TEXT("/C"),path))
        {
            SetShaderLibraryPath(path);
            return(0);
        }
    }

    OSString input_filename = argv[1];
    OSString output_filename;
    OSString ext_name=OS_TEXT(".material");
    
    if(argc<3)  output_filename=ReplaceExtName(input_filename,ext_name);
        else    output_filename=argv[2];

    LOG_INFO(OS_TEXT("Input File: ") + input_filename);
    LOG_INFO(OS_TEXT("Output File: ") + output_filename);

    glsl_compiler::Init();

    LoadMat(input_filename);

    glsl_compiler::Close();
    return 0;
}
