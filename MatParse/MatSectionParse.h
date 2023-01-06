#pragma once
#include"MatSection.h"
#include<hgl/type/String.h>

using namespace hgl;

class MatSectionParse
{
public:
        
    virtual const MatSection GetSection()const=0;

public:

    MatSectionParse()=default;
    virtual ~MatSectionParse() = default;

    virtual void Add(const UTF8String &str,const UTF8String &raw) = 0;
};//class MatSectionParse

MatSectionParse *CreateMSP(const MatSection &);
