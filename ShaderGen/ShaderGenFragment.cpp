#include"ShaderGen.h"
#include<hgl/filesystem/FileSystem.h>

class ShaderGenFragment:public ShaderGen
{
public:
    
    ShaderGenFragment(ShaderDataManager *_sdm, const OSString &path,ShaderGen *prev):ShaderGen(_sdm, path,prev) {}
    
    void PreProcess() override
    {
        if(!prev_gen)return;

        int count=prev_gen->GetStageOutputCount();

        if(count<=0)return;

        AddInclude(prev_gen->GetOutputStructFilename());
    }

    bool Begin() override
    {
        if(prev_gen&&prev_gen->GetStageOutputCount()>0)
            Add("layout(location=0) in "+prev_gen->GetOutputStructName()+"\tInput;\n");

        return(true);
    }

    bool End() override
    {
        return(true);
    }    
};//class ShaderGenFragment:public ShaderGen

ShaderGen *CreateShaderGenFragment(ShaderDataManager *sdm,const OSString &path,ShaderGen *prev)
{
    return(new ShaderGenFragment(sdm,path,prev));
}