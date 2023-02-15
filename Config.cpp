#include<hgl/filesystem/FileSystem.h>
#include<hgl/util/plist/PList.h>

using namespace hgl;

namespace
{
    OSString cur_work_path;
    OSString cfg_filename;

    UTF8String shader_library_path;
}

const UTF8String GetShaderLibraryPath()
{
    return shader_library_path;
}

void SaveConfig()
{
    UTF8PList cfg_plist;

    cfg_plist.Add("ShaderLibraryPath", shader_library_path);

    if(!cfg_plist.SaveToTextFile<ByteOrderMask::UTF16LE>(cfg_filename))
    {
        LOG_ERROR(OS_TEXT("Save config file failure."));
        return;
    }

    LOG_INFO(OS_TEXT("Save config file success."));
}

bool LoadConfig()
{
    if(!filesystem::GetCurrentPath(cur_work_path))
    {
        LOG_ERROR(OS_TEXT("can't get local appdata path."));
        return(false);
    }

    cfg_filename=filesystem::MergeFilename(cur_work_path,OS_TEXT("ShaderBuilder.cfg"));

    LOG_INFO(OS_TEXT("Config File: ")+cfg_filename);

    if(!filesystem::FileExist(cfg_filename))
    {
        LOG_ERROR(OS_TEXT("Config File not exist. Create a new ShaderBuilder.cfg"));
        SaveConfig();
        return(false);
    }

    UTF8PList cfg_plist;

    if(!cfg_plist.LoadFromTextFile(cfg_filename))
    {
        LOG_ERROR(OS_TEXT("Load config file failure."))
        return(false);
    }
    else
    {
        LOG_ERROR(OS_TEXT("Loaded config file OK."))
    }

    if(cfg_plist.Get("ShaderLibraryPath",shader_library_path))
    {
        LOG_INFO(U8_TEXT("ShaderLibraryPath: ") + shader_library_path);
    }
    else
    {
        LOG_ERROR(OS_TEXT("ShaderLibraryPath not found."));
        return(false);
    }
    
    return(true);
}
