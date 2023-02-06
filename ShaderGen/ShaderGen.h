#pragma once
#include"ShaderData/ShaderDataManager.h"
#include<hgl/type/StringList.h>

class ShaderGen
{
protected:

    ShaderGen *prev_gen;

    ShaderDataManager *sdm;
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

    const int GetStageOutputCount()const;

    const UTF8String &GetOutputStructName()const{return output_struct_name;}
    const UTF8String &GetOutputStructFilename()const{return output_struct_filename;}

public:

    ShaderGen(ShaderDataManager *_sdm,const OSString &path,ShaderGen *prev=nullptr);
    virtual ~ShaderGen()=default;

    const ShaderGen *GetPrevShadeGen()const{return prev_gen;}

    virtual bool Gen();
};//class ShaderGen

ShaderGen *CreateShaderGen(ShaderDataManager *sdm,const OSString &,ShaderGen *prev=nullptr);
