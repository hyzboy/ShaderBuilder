#pragma once

#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>
#include<hgl/graph/VKShaderStage.h>
#include"MaterialDescriptorManager.h"

using namespace hgl::graph;

/**
* Shader数据管理器,用于生成正式Shader前的资源统计
*/
class ShaderDescriptorManager
{
    ShaderStageIO stage_io;
    
    MaterialDescriptorManager *         material_descriptor_manager;

    //ubo/object在这里以及MaterialDescriptorManager中均有一份，mdm中的用于产生set/binding号，这里的用于产生shader
    List<const UBODescriptor *>         ubo_list;
    List<const ShaderObjectData *>      object_list;
    
    ObjectList<ConstValueDescriptor>    const_value_list;
    ObjectList<SubpassInputDescriptor>  subpass_input;
    
    ShaderPushConstant                  push_constant;

    UTF8String                          source_codes;

public:

    ShaderDescriptorManager(VkShaderStageFlagBits,MaterialDescriptorManager *);
    ~ShaderDescriptorManager()=default;

    const VkShaderStageFlagBits GetStageBits()const { return stage_io.cur; }
    const UTF8String GetStageName()const { return UTF8String(GetShaderStageName(stage_io.cur)); }
   
    void SetPrevShader(VkShaderStageFlagBits prev) { stage_io.prev=prev; }
    void SetNextShader(VkShaderStageFlagBits next) { stage_io.next=next; }

public:

    const ShaderStageIO &GetShaderStageIO()const{return stage_io;}

    const List<const UBODescriptor *> &GetUBOList()const{return ubo_list;}
    const List<const ShaderObjectData *> &GetObjectList()const { return object_list; }

    const ObjectList<ConstValueDescriptor> &GetConstList()const{return const_value_list;}

    const ObjectList<SubpassInputDescriptor> &GetSubpassInputList()const { return subpass_input; }

    const UTF8String &GetSources()const{return source_codes;}
    
public:

    bool AddInput(ShaderStage *);
    bool AddOutput(ShaderStage *);

    bool AddUBO(DescriptorSetType type,UBODescriptor *sd);
    bool AddObject(DescriptorSetType type,ShaderObjectData *sd);

    bool AddConstValue(ConstValueDescriptor *sd);    
    bool AddSubpassInput(const UTF8String name,uint8_t index);
    
    void SetPushConstant(const UTF8String name,uint8_t offset,uint8_t size);

    void AddSource(const UTF8String &str)
    {
        source_codes+=str;
        source_codes+="\n";
    }

#ifdef _DEBUG
    void DebugOutput(int);
#endif//_DEBUG
};//class ShaderDescriptorManager
