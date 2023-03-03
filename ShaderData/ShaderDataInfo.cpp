#include"ShaderDataInfo.h"
#include"ShaderDescriptorManager.h"
#include"ShaderGen/ShaderGen.h"
#include"GLSLCompiler/GLSLCompiler.h"
#include<hgl/log/LogInfo.h>

ShaderDataInfo::ShaderDataInfo(const VkShaderStageFlagBits ssb,const OSString &fn)
{
    shader_stage_bit=ssb;
    filename=fn;
    sdm=nullptr;
    sg=nullptr;
    spv=nullptr;

    LOG_INFO(UTF8String("Create ")+GetShaderStageName(shader_stage_bit)+UTF8String(" shader data info from file: ")+to_u8(filename));
}

ShaderDataInfo::~ShaderDataInfo()
{
    SAFE_CLEAR(sdm);
    SAFE_CLEAR(sg);

    if(spv)
        glsl_compiler::Free(spv);

    LOG_INFO(UTF8String("Free ")+GetShaderStageName(shader_stage_bit)+UTF8String(" shader data info from file: ")+to_u8(filename));
}