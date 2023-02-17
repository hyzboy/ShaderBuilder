#include"ShaderDataInfo.h"
#include"ShaderDataManager.h"
#include"ShaderGen/ShaderGen.h"
#include"GLSLCompiler/GLSLCompiler.h"
#include<hgl/log/LogInfo.h>

ShaderDataInfo::ShaderDataInfo(const vk_shader::ShaderStageBits ssb,const OSString &fn)
{
    shader_stage_bit=ssb;
    filename=fn;
    shader_data_manager=nullptr;
    sg=nullptr;
    spv=nullptr;

    LOG_INFO(UTF8String("Create ")+vk_shader::GetShaderStageName(shader_stage_bit)+UTF8String(" shader data info from file: ")+to_u8(filename));
}

ShaderDataInfo::~ShaderDataInfo()
{
    SAFE_CLEAR(shader_data_manager);
    SAFE_CLEAR(sg);

    if(spv)
        glsl_compiler::Free(spv);

    LOG_INFO(UTF8String("Free ")+vk_shader::GetShaderStageName(shader_stage_bit)+UTF8String(" shader data info from file: ")+to_u8(filename));
}