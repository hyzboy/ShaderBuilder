#include<hgl/type/LoadStringList.h>
#include<hgl/type/Map.h>
#include<hgl/type/SplitString.h>
#include<hgl/filesystem/FileSystem.h>
#include"GLSLCompiler/GLSLCompiler.h"
#include"vulkan/VKShaderCommon.h"
#include"ShaderData/ShaderDataManager.h"
#include"ShaderGen/ShaderGen.h"

using namespace hgl;

bool LoadShader(ShaderDataManager *sdm,const UTF8StringList &source_codes);

ShaderGen *CreateShaderGen(ShaderDataManager *sdm,const OSString &);

bool LoadMat(const OSString &filename,const OSString &output_path)
{
    UTF8StringList sl;

    if(!LoadStringListFromTextFile(sl,filename))
    {
        LOG_ERROR(OS_TEXT("Load .Mat file failure. filename: ")+filename);
        return(false);
    }

    UTF8String pmc;
    Map<vk_shader::ShaderStageBits,OSString> shaderfile;

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
            vk_shader::ShaderStageBits type=glsl_compiler::GetType(left);

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

    MaterialDescriptorManager MDM;
    ObjectList<ShaderDataManager> SDMList;
    
    if(!shaderfile.KeyExist(vk_shader::ssbFragment))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);
    }

    if(!shaderfile.KeyExist(vk_shader::ssbVertex))
    {
        LOG_ERROR("can't find fragment shader.");
        return(false);        
    }

    {
        auto *dp=shaderfile.GetDataList();
        
        for(int i=0;i<shaderfile.GetCount();i++)
        {   
            UTF8StringList codes;

            if(!LoadStringListFromTextFile(codes, (*dp)->right))
            {
                LOG_ERROR(OS_TEXT("Load shader file failure. filename: ")+(*dp)->right);
                continue;
            }
            
            ShaderDataManager *sdm=new ShaderDataManager((*dp)->left, &MDM);

            if(!LoadShader(sdm, codes))
            {
                delete sdm;
                return(false);
            }

            SDMList.Add(sdm);

            ++dp;
        }
    }

    MDM.Resort();
    ResortShader(SDMList);

    {
        for(auto sdm:SDMList)
        {            
            ShaderGen *sg=CreateShaderGen(sdm,output_path);

            if(sg)
            {
                sg->Gen();

                delete sg;
            }
            else
            {
                LOG_ERROR(UTF8String("Create ShaderGen failure : ")+GetShaderStageName(sdm->GetStageBits()));
                return(false);
            }
        }
    }

    return(true);
}