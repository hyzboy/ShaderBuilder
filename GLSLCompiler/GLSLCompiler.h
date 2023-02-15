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
    
    constexpr size_t SHADER_RESOURCE_NAME_MAX_LENGTH=128;

    struct ShaderResource
    {
        char name[SHADER_RESOURCE_NAME_MAX_LENGTH];

        uint8_t set;
        uint8_t binding;

    public:

        ShaderResource(const ShaderResource *sr)
        {
            memcpy(name,sr->name,SHADER_RESOURCE_NAME_MAX_LENGTH);
            set=sr->set;
            binding=sr->binding;
        }

        const int Comp(const ShaderResource &sr)const
        {
            if(set!=sr.set)return sr.set-set;
            if(binding!=sr.binding)return sr.binding-binding;

            return strcmp(name,sr.name);
        }

        CompOperator(const ShaderResource &,Comp);
    };//struct ShaderResource

    struct ShaderStage
    {
        char name[SHADER_RESOURCE_NAME_MAX_LENGTH];
        uint8_t location;
        uint32_t basetype;      //现在改对应hgl/graph/VertexAttrib中的enum class VertexAttribBaseType
        uint32_t vec_size;
    };//

    struct ShaderStageData
    {
        uint32_t count;
        ShaderStage *items;
    };

    struct ShaderResourceData
    {
        uint32_t count;
        ShaderResource *items;
    };

    using ShaderFullResourceData=ShaderResourceData[size_t(DescriptorType::RANGE_SIZE)];

    struct SPVData
    {
        bool result;
        char *log;
        char *debug_log;

        uint32_t *spv_data;
        uint32_t spv_length;

        ShaderStageData input,output;
        ShaderFullResourceData resource;

        ShaderResourceData push_constant;
        ShaderResourceData subpass_input;
    };

    bool Init();
    void Close();

    void        AddGLSLIncludePath(const char *);    
    void        RebuildGLSLIncludePath();

    uint32_t    GetType (const char *ext_name);
    
    SPVData *   Compile (const uint32_t type,const char *source);
    void        Free    (SPVData *spv_data);
}//namespace glsl_compiler
#endif//HGL_GLSL_COMPILER_INCLUDE
