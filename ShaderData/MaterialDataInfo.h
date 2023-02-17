#pragma once

#include"ShaderData/ShaderDataInfo.h"
#include"ShaderData/MaterialDescriptorManager.h"

struct MaterialDataInfo
{
    OSString filename;

    OSString output_path;

    UTF8String pmc;

    MaterialDescriptorManager MDM;
    
    ShaderMap shader_map;
};
