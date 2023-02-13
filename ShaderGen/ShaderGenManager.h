#pragma once

#include"ShaderData/ShaderDataManager.h"

struct ConvertMaterialData
{
    OSString filename;

    OSString output_path;

    UTF8String pmc;

    Map<vk_shader::ShaderStageBits,OSString> shaderfile;

    MaterialDescriptorManager MDM;
    ObjectList<ShaderDataManager> SDMList;
};
