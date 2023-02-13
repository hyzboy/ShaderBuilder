#include"ShaderGenManager.h"
#include"ShaderGen.h"

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

    {
        ShaderGen *prev=nullptr;
        ObjectList<ShaderGen> SGList;
        
        for(auto sdm:cvd.SDMList)
        {            
            ShaderGen *sg=CreateShaderGen(sdm,output_path,prev);

            if(sg)
            {
                sg->Gen();

                SGList.Add(sg);

                prev=sg;
            }
            else
            {
                LOG_ERROR(UTF8String("Create ShaderGen failure : ")+sdm->GetStageName());
                return(false);
            }
        }
    }

    return(true);
}