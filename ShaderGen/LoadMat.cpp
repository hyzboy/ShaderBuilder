#include<hgl/type/LoadStringList.h>
#include<hgl/type/Map.h>
#include<hgl/type/SplitString.h>
#include<hgl/filesystem/FileSystem.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"vulkan/VKShaderCommon.h"
#include"ShaderGenManager.h"
#include"ShaderGen/ShaderGen.h"

using namespace hgl;

bool LoadMat(ConvertMaterialData *cvd)
{
    if(!cvd)return(false);

    UTF8StringList sl;

    if(!LoadStringListFromTextFile(sl,cvd->filename))
    {
        LOG_ERROR(OS_TEXT("Load .Mat file failure. filename: ")+cvd->filename);
        return(false);
    }

    UTF8String left,right;
    OSString path;

    path=filesystem::ClipPathname(cvd->filename,false);

    for(int i=0;i<sl.GetCount();i++)
    {
        SpliteByString(sl[i],UTF8String(u8":"),&left,&right);

        left=left.Trim();
        right=right.Trim();

        if(left.IsEmpty()||right.IsEmpty())
            continue;

        if(left.CaseComp(u8"pmc")==0)
            cvd->pmc=right;
        else
        {
            uint32_t type=glsl_compiler::GetType(left);

            if(type==0)
                continue;
            
            OSString fullname=filesystem::MergeFilename(path,ToOSString(right+U8_TEXT(".")+left));

            if(!filesystem::FileExist(fullname))
            {
                LOG_ERROR(OS_TEXT("Shader file not exist. filename: ")+fullname);
                return(false);
            }
            
            cvd->shaderfile.Add((vk_shader::ShaderStageBits)type,fullname);
        }
    }

    return(true);
}