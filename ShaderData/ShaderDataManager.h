#pragma once

#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>

using namespace hgl;
using namespace vk_shader;

class ShaderDataManager
{
    ShaderStageBits ssb;

    char *spv_data;
    uint32_t spv_length;
    AnsiString compiled_log;

    ShaderStageIO stage_io;
    
    ObjectList<ShaderDescriptor> descriptor_list;
    List<ShaderUBOData *> ubo_list;
    List<ShaderObjectData *> object_list;
    ObjectList<ShaderConstValue> const_value_list;
    ShaderSubpassInput subpass_input;
    ShaderPushConstant push_constant;

private:
        
    ShaderDescriptor *GetDescriptor(const char *name);                                              ///<�������ƻ�ȡһ��������
    bool AddDescriptor(ShaderStageBits bit,DescriptorSetsType set_type,ShaderDescriptor *new_sd);   ///<���һ���������������������ڣ��򷵻�false

public:

    ShaderDataManager(ShaderStageBits cur,ShaderStageBits prev,ShaderStageBits next)
    {
        ssb=cur;

        spv_data=nullptr;
        spv_length=0;
        
        stage_io.cur=cur;
        stage_io.prev=prev;
        stage_io.next=next;
    }

    ~ShaderDataManager()
    {
        Clear();
    }

    void Clear()
    {
        SAFE_CLEAR_ARRAY(spv_data);
        spv_length=0;
        compiled_log.Clear();

        descriptor_list.Clear();       
        ubo_list.Clear();
        object_list.Clear();
        const_value_list.Clear();
        hgl_zero(subpass_input);
        hgl_zero(push_constant);
    }


    bool AddUBO(DescriptorSetsType type,ShaderUBOData *sd)
    {
        ENUM_CLASS_RANGE_ERROR_RETURN_FALSE(type);
        if(!sd)return(false);


    }

    void AddObject(DescriptorSetsType type,ShaderObjectData *sd);

    void AddConstValue(ShaderConstValue *sd);

    void AddSubpassInput(const UTF8String name,uint8_t index)
    {
        //����Ҫ������ֺ��������Ƿ��ظ�

        subpass_input.name=name;
        subpass_input.input_attachment_index=index;
    }

    void SetPushConstant(const UTF8String name,uint8_t offset,uint8_t size)
    {
        push_constant.name  =name;
        push_constant.offset=offset;
        push_constant.size  =size;
    }

};//class ShaderDataManager
