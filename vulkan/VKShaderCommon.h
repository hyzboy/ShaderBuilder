#pragma once

#include<stdint.h>
#include<hgl/TypeFunc.h>
#include<hgl/CompOperator.h>
#include<hgl/type/List.h>

using namespace hgl;

namespace vk_shader
{
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
};//namespace vk_shader
