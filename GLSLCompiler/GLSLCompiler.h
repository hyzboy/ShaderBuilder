#ifndef HGL_GLSL_COMPILER_INCLUDE
#define HGL_GLSL_COMPILER_INCLUDE

#include<hgl/type/String.h>
#include<hgl/type/Map.h>
#include<stdint.h>

#include"vulkan/VKShaderCommon.h"

namespace hgl
{
    namespace io
    {
        class MemoryOutputStream;
        class DataOutputStream;
    }
}

namespace glsl_compiler
{
    using namespace hgl;
    using namespace vk_shader;
    
    struct SPVData
    {
        bool result;
        char *log;
        char *debug_log;

        uint32_t *spv_data;
        uint32_t spv_length;

        ShaderStageIO stage_io;
        ShaderSubpassInput subpass_input;
        ShaderPushConstant push_constant;
        ObjectList<ShaderDescriptor> descriptor_sets[size_t(DescriptorType::RANGE_SIZE)];
    };

    bool Init();
    void Close();

    void        AddGLSLIncludePath(const char *);
    void        RebuildGLSLIncludePath();

    ShaderType  GetType (const char *ext_name);
    
    SPVData *   Compile (const uint32_t type,const char *source);
    void        Free    (SPVData *spv_data);

    SPVData *   CompileShaderToSPV(const uint8 *source,const uint32_t flag);
}//namespace glsl_compiler
#endif//HGL_GLSL_COMPILER_INCLUDE
