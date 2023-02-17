#pragma once
#include"ShaderSection.h"
#include<hgl/type/String.h>

using namespace hgl;

class ShaderDescriptorManager;

class ShaderSectionParse
{
protected:

    ShaderDescriptorManager *sdm;
    
public:

    virtual const ShaderSection GetSection()const=0;

public:

    ShaderSectionParse(ShaderDescriptorManager *s){sdm=s;}
    virtual ~ShaderSectionParse() = default;

    virtual void Add(const UTF8String &str,const UTF8String &raw) = 0;
};//class ShaderSectionParse

ShaderSectionParse *CreateSSP(const ShaderSection &,ShaderDescriptorManager *);
