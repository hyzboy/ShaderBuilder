#pragma once

#include"vulkan/VKShaderCommon.h"
#include<hgl/type/Map.h>
#include"MaterialDescriptorManager.h"
    
/**
* Shader数据管理器,用于生成正式Shader前的资源统计
*/
class ShaderDataManager
{
    ShaderStageIO stage_io;
    
    MaterialDescriptorManager *     descriptor_set_manager;

    //ubo/object在这里以及MaterialDescriptorManager中均有一份，mdm中的用于产生set/binding号，这里的用于产生shader
    List<const ShaderUBOData *>     ubo_list;
    List<const ShaderObjectData *>  object_list;
    
    ObjectList<ShaderConstValue>    const_value_list;
    ObjectList<ShaderSubpassInput>  subpass_input;
    
    ShaderPushConstant              push_constant;

    UTF8String                      source_codes;

public:

    ShaderDataManager(ShaderStageBits,MaterialDescriptorManager *);
    ~ShaderDataManager()=default;

    const ShaderStageBits GetStageBits()const { return stage_io.cur; }
    const UTF8String GetStageName()const { return UTF8String(GetShaderStageName(stage_io.cur)); }
   
    void SetPrevShader(ShaderStageBits prev) { stage_io.prev=prev; }
    void SetNextShader(ShaderStageBits next) { stage_io.next=next; }

public:

    const ShaderStageIO &GetShaderStageIO()const{return stage_io;}

    const List<const ShaderUBOData *> &GetUBOList()const{return ubo_list;}
    const List<const ShaderObjectData *> &GetObjectList()const { return object_list; }

    const ObjectList<ShaderConstValue> &GetConstList()const{return const_value_list;}

    const ObjectList<ShaderSubpassInput> &GetSubpassInputList()const { return subpass_input; }

    const UTF8String &GetSources()const{return source_codes;}
    
public:

    bool AddInput(ShaderStage *);
    bool AddOutput(ShaderStage *);

    bool AddUBO(DescriptorSetType type,ShaderUBOData *sd);
    bool AddObject(DescriptorSetType type,ShaderObjectData *sd);

    bool AddConstValue(ShaderConstValue *sd);    
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
};//class ShaderDataManager

struct ConvertMaterialData;
void ResortShader(ConvertMaterialData *);
