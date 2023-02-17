#pragma once
#include"ShaderData/ShaderDescriptorManager.h"
#include<hgl/type/StringList.h>

constexpr char TOP_COMMENT[]=u8"// the Shader was created by the ShaderBuilder (" HGL_OFFICAL_WEB_U8 u8")\n\n";

class ShaderGen
{
protected:

    ShaderGen *prev_gen;

    ShaderDescriptorManager *sdm;
    OSString output_path;
    
    UTF8String shader_source;

    UTF8String output_struct_name;
    UTF8String output_struct_filename;

protected:

    void Add(const UTF8String &);
    void AddLineBreak();

    void AddStageOutput();

protected:

    UTF8StringList include_list;
    
    void AddInclude(const UTF8String &fn){include_list.AddUnique(fn);}

    void ProcInclude();

    void ProcSubpassInput();

    void ProcUBO();
    void ProcObject();
    void ProcConst();

protected:

    virtual void PreProcess()=0;

    virtual bool Begin()=0;
    virtual bool End()=0;

public:

    const ShaderStageBits GetStageBits()const{return sdm->GetStageBits();}

    const int GetStageOutputCount()const;

    const UTF8String &GetOutputStructName()const{return output_struct_name;}
    const UTF8String &GetOutputStructFilename()const{return output_struct_filename;}

public:

    ShaderGen(ShaderDescriptorManager *_sdm,const OSString &path,ShaderGen *prev=nullptr);
    virtual ~ShaderGen()=default;

    const ShaderGen *GetPrevShadeGen()const{return prev_gen;}

    virtual bool Gen();

    const UTF8String &GetShaderSource()const{return shader_source;}
};//class ShaderGen

ShaderGen *CreateShaderGen(ShaderDescriptorManager *sdm,const OSString &,ShaderGen *prev=nullptr);
