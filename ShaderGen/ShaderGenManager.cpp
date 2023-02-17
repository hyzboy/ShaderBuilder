#include"ShaderGenManager.h"
#include"ShaderGen.h"

#include"GLSLCompiler/GLSLCompiler.h"

bool LoadMat(ConvertMaterialData *);
bool LoadShader(ConvertMaterialData *);

bool ConvertMaterial(const OSString &filename,const OSString &output_path)
{
    ConvertMaterialData cvd;

    cvd.filename=filename;
    cvd.output_path=output_path;

    if(!LoadMat(&cvd))
        return(false);

    if(!LoadShader(&cvd))
        return(false);

    cvd.MDM.Resort();

    List<SDMPointer> SDMList;
    {
        auto **it=cvd.shader_map.GetDataList();
        for(int i=0;i<cvd.shader_map.GetCount();i++)
        {
            SDMList.Add((*it)->right->shader_data_manager);
            ++it;
        }
    }
 
    ResortShader(SDMList);

    {
        ShaderDataInfo *sdi;
        ShaderGen *prev=nullptr;

        for(auto sdm:SDMList)
        {
            if(!cvd.shader_map.Get(sdm->GetStageBits(),sdi))
            {
                LOG_ERROR(UTF8String("Can't find ShaderDataInfo for ")+sdm->GetStageName());
                return(false);  
            }

            sdi->sg=CreateShaderGen(sdm,output_path,prev);

            if(sdi->sg)
            {
                sdi->sg->Gen();

                glsl_compiler::SPVData *spv=glsl_compiler::Compile(sdi->sg->GetStageBits(),sdi->sg->GetShaderSource().c_str());

                if(spv&&spv->result)
                    sdi->spv=spv;
                else
                {
                    LOG_ERROR(UTF8String("Compile ")+sdm->GetStageName()+UTF8String(" shader failure."));

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
                LOG_ERROR(UTF8String("Create ShaderGen failure: ")+sdm->GetStageName());
                return(false);
            }
        }
    }

    return(true);
}