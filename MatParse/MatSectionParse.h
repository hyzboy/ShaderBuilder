#pragma once
#include"MatSection.h"
#include<hgl/type/String.h>

using namespace hgl;

class MatSectionParse
{
public:
        
    virtual const MatSection GetSection()const=0;

public:

    MatSectionParse(){}
    virtual ~MatSectionParse()=0;

    virtual void Add(const UTF8String &str) = 0;
};//class MatSectionParse
