#pragma once
#include<hgl/type/String.h>
#include<hgl/type/List.h>

using namespace hgl;

struct Varying
{
    UTF8String type;
    UTF8String name;
    UTF8String interpolation;
    UTF8String comment;
};//

using VaryingList=ObjectList<Varying>;
