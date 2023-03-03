#include"ShaderData/MaterialDataInfo.h"
#include"ShaderGen/ShaderGen.h"
#include"GLSLCompiler/GLSLCompiler.h"

bool CompileToSPV(MaterialDataInfo &mdi)
{
    ShaderDataInfo *sdi;
    ShaderGen *prev=nullptr;

    UTF8String stage_name;

    auto *sp=mdi.shader_map.GetDataList();
    const int count=mdi.shader_map.GetCount();

    for(int i=0;i<count;i++)
    {
        sdi=(*sp)->right;

        stage_name=GetShaderStageName(sdi->shader_stage_bit);

        if(!mdi.shader_map.Get(sdi->shader_stage_bit,sdi))
        {
            LOG_ERROR(UTF8String("Can't find ShaderDataInfo for ")+stage_name);
            return(false);
        }

        sdi->sg=CreateShaderGen(sdi->sdm,mdi.output_path,prev);

        if(sdi->sg)
        {
            sdi->sg->Gen();

            glsl_compiler::SPVData *spv=glsl_compiler::Compile(sdi->sg->GetStageBits(),sdi->sg->GetShaderSource().c_str());

            if(spv&&spv->result)
                sdi->spv=spv;
            else
            {
                LOG_ERROR(UTF8String("Compile ")+stage_name+UTF8String(" shader failure."));

                if(spv)
                {
                    LOG_ERROR(UTF8String("Compile log: ")+spv->log);
                    LOG_ERROR(UTF8String("Compile debug log: ")+spv->debug_log);    
                }
            }

            prev=sdi->sg;
        }
        else
        {
            LOG_ERROR(UTF8String("Create ShaderGen failure: ")+stage_name);
            return(false);
        }

        ++sp;
    }

    return(true);
}