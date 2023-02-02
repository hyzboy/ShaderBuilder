#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/plist/PList.h>

using namespace hgl;
using namespace hgl::filesystem;

namespace
{
    OSString cfg_filename;
    OSString shader_library_path;
}

bool SetShaderLibraryPath(const OSString &path)
{
    if(!IsDirectory(path))
        return(false);
    
    shader_library_path=path;
    SaveConfig();
    return(true);
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