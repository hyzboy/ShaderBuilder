#pragma once
#include"ShaderSection.h"
#include<hgl/type/String.h>

using namespace hgl;

class ShaderDataManager;

class ShaderSectionParse
{
protected:

    ShaderDataManager *sdm;
    
public:

    virtual const ShaderSection GetSection()const=0;

public:

    ShaderSectionParse(ShaderDataManager *s){sdm=s;}
    virtual ~ShaderSectionParse() = default;

    virtual void Add(const UTF8String &str,const UTF8String &raw) = 0;
};//class ShaderSectionParse

ShaderSectionParse *CreateSSP(const ShaderSection &,ShaderDataManager *);
