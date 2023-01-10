#include<hgl/log/LogInfo.h>
#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/plist/PList.h>
#include<hgl/util/cmd/CmdParse.h>
#include<hgl/Endian.h>
#include"GLSLCompiler/GLSLCompiler.h"

using namespace hgl;
using namespace hgl::filesystem;

namespace
{
    OSString cfg_filename;
    OSString shader_library_path;
}

bool LoadConfig()
{
    OSString path;
    
    if(!GetLocalAppdataPath(path))
    {
        LOG_ERROR(OS_TEXT("can't get local appdata path."));
        return(false);
    }

    cfg_filename=MergeFilename(path,OS_TEXT(".cmgdk") HGL_DIRECTORY_SEPARATOR_STR OS_TEXT("ShaderBuilder.cfg"));

    LOG_INFO(OS_TEXT("Config File: ")+cfg_filename);

    if(!FileExist(cfg_filename))
    {
        LOG_ERROR(OS_TEXT("Config File not exist. please create it that use /C."));
        return(false);
    }

    OSPList cfg_plist;

    if(!cfg_plist.LoadFromTextFile(cfg_filename))
    {
        LOG_ERROR(OS_TEXT("Load config file failure."))
        return(false);
    }
    else
    {
        LOG_ERROR(OS_TEXT("Loaded config file OK."))
    }

    if(cfg_plist.Get(OS_TEXT("ShaderLibraryPath"),shader_library_path))
    {
        LOG_INFO(OS_TEXT("ShaderLibraryPath: ") + shader_library_path);
    }
    else
    {
        LOG_ERROR(OS_TEXT("ShaderLibraryPath not found."));
        return(false);
    }
    
    return(true);
}

void SaveConfig()
{
    OSPList cfg_plist;

    cfg_plist.Add(OS_TEXT("ShaderLibraryPath"), shader_library_path);

    if(!cfg_plist.SaveToTextFile<ByteOrderMask::UTF16LE>(cfg_filename))
    {
        LOG_ERROR(OS_TEXT("Save config file failure."));
        return;
    }

    LOG_INFO(OS_TEXT("Save config file success."));
}

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
            shader_library_path = path;

            SaveConfig();
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
