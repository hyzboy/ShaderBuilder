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
    ResortShader(cvd.SDMList);

    ObjectMap<ShaderStageBits,glsl_compiler::SPVData> spv_map;

    {
        ShaderGen *prev=nullptr;
        ObjectList<ShaderGen> SGList;
        
        for(auto sdm:cvd.SDMList)
        {            
            ShaderGen *sg=CreateShaderGen(sdm,output_path,prev);

            if(sg)
            {
                sg->Gen();

                glsl_compiler::SPVData *spv=glsl_compiler::Compile(sg->GetStageBits(),sg->GetShaderSource().c_str());

                if(spv&&spv->result)
                    spv_map.Add(sg->GetStageBits(),spv);
                else
                {
                    LOG_ERROR(UTF8String("Compile ")+sdm->GetStageName()+UTF8String(" shader failure."));
                    if(spv)
                    {
                        LOG_ERROR(UTF8String("Compile log: ")+spv->log);
                        LOG_ERROR(UTF8String("Compile debug log: ")+spv->debug_log);    
                    }
                }

                SGList.Add(sg);

                prev=sg;
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