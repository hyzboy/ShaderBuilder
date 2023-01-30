#pragma once

#include<stdint.h>
#include<hgl/TypeFunc.h>
#include<hgl/CompOperator.h>
#include<hgl/type/List.h>

using namespace hgl;

namespace vk_shader
{
    enum ShaderStageBits        //等同VkShaderStageFlagBits
    {
        ssbVertex       = 0x00000001,
        ssbTessControl  = 0x00000002,
        ssbTessEval     = 0x00000004,
        ssbGeometry     = 0x00000008,
        ssbFragment     = 0x00000010,
        ssbGraphics     = 0x0000001F,
        ssbCompute      = 0x00000020,
        ssbTask         = 0x00000040,
        ssbMesh         = 0x00000080,
        ssbRaygen       = 0x00000100,
        ssbAnyHit       = 0x00000200,
        ssbClosestHit   = 0x00000400,
        ssbMiss         = 0x00000800,
        ssbIntersection = 0x00001000,
        ssbCallable     = 0x00002000,
        ssbAll          = 0x7FFFFFFF
    };

    using ShaderType=uint32_t;          ///<用来单独表示SHADER是那一种
    
    constexpr const char *shader_stage_name_list[32]=
    {
        "Vertex"      ,
        "TessControl" ,
        "TeseEval"    ,
        "Geometry"    ,
        "Fragment"    ,
        "Compute"     ,
        "Task"        ,
        "Mesh"        ,
        "Raygen"      ,
        "AnyHit"      ,
        "ClosestHit"  ,
        "Miss"        ,
        "Intersection",
        "Callable"
    };//

    const char *GetShaderStageName(const ShaderType &);
    
    enum class DescriptorType         //等同VkDescriptorType
    {
        SAMPLER = 0,
        COMBINED_IMAGE_SAMPLER,
        SAMPLED_IMAGE,
        STORAGE_IMAGE,
        UNIFORM_TEXEL_BUFFER,
        STORAGE_TEXEL_BUFFER,
        UNIFORM_BUFFER,
        STORAGE_BUFFER,
        UNIFORM_BUFFER_DYNAMIC,
        STORAGE_BUFFER_DYNAMIC,
        INPUT_ATTACHMENT,

        ENUM_CLASS_RANGE(SAMPLER,INPUT_ATTACHMENT)
    };

    constexpr const char *descriptor_type_name[32]=
    {
        "sampler",
        "combined_image_sampler",
        "sampled_image",
        "storage_image",
        "uniform_texel_buffer",
        "storage_texel_buffer",
        "uniform_buffer",
        "storage_buffer",
        "uniform_buffer_dynamic",
        "storage_buffer_dynamic",
        "input_attachment",

        "push_constant",
        "subpass_input"
    };
    
    enum class DescriptorSetsType
    {
        Global=0,   ///<全局参数(如太阳光等)
        PerFrame,   ///<每帧参数(如摄像机等)
        PerObject,  ///<每个物体参数(如模型矩阵等)
        Instance,

        ENUM_CLASS_RANGE(Global,Instance)
    };//enum class DescriptorSetsType
    
    constexpr size_t DESCRIPTOR_NAME_MAX_LENGTH=128;

    struct ShaderDescriptor
    {
        char name[DESCRIPTOR_NAME_MAX_LENGTH];
        vk_shader::DescriptorType type;
        DescriptorSetsType set_type;

        int set;
        int binding;
        uint32_t stage_flag;

    public:

        ShaderDescriptor()
        {
            hgl_zero(*this);
            set=-1;
            binding=-1;
        }
        
        ShaderDescriptor(const ShaderDescriptor *sr)
        {
            if(!sr)
            {
                hgl_zero(*this);

                set=-1;
                binding=-1;
            }
            else
                hgl_cpy(*this,*sr);
        }

        const int Comp(const ShaderDescriptor &sr)const
        {
            if(set!=sr.set)return sr.set-set;
            if(binding!=sr.binding)return sr.binding-binding;

            return strcmp(name,sr.name);
        }

        CompOperator(const ShaderDescriptor &,Comp);
    };//struct ShaderDescriptor
    
    struct ShaderStage
    {
        char name[DESCRIPTOR_NAME_MAX_LENGTH];
        uint8_t location;
        uint32_t basetype;      //现在改对应hgl/graph/VertexAttrib中的enum class VertexAttribBaseType
        uint32_t vec_size;
    };//
    
    struct ShaderStageIO
    {
        uint prev,cur,next;
        ObjectList<ShaderStage> input,output;

    public:
        
        ShaderStageIO()
        {
            prev=cur=next=0;
        }
    };
    
    struct ShaderUBOData:public ShaderDescriptor
    {
        UTF8String type;
        UTF8String name;
    };

    struct ShaderObjectData:public ShaderDescriptor
    {
        UTF8String type;
        UTF8String name;
    };

    struct ShaderConstValue:public ShaderDescriptor
    {
        UTF8String type;
        UTF8String name;
        UTF8String value;
    };

    struct ShaderSubpassInput
    {
        UTF8String name;
        uint8_t input_attachment_index;
    };

    struct ShaderPushConstant
    {
        UTF8String name;
        uint8_t offset;
        uint8_t size;
    };
};//namespace vk_shader
