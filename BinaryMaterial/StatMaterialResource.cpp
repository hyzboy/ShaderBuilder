#include"StatMaterialResource.h"
#include"ShaderData/ShaderDataInfo.h"

const int MaterialDescriptorResource::Comp(const MaterialDescriptorResource &msr)const
{
    int result;

    result=desc->set-msr.desc->set;
    if(result)return result;
        
    result=desc->binding-msr.desc->binding;
    if(result)return result;

    result=strcmp(desc->name,msr.desc->name);
    if(result)return result;

    result=shader_stage_bits-msr.shader_stage_bits;
    if(result)return result;

    return(desc_type-msr.desc_type);
}

void StatDescriptorResource(MDRList &mdr_list,const vk_shader::ShaderStageBits &ssb,const glsl_compiler::ShaderResourceData<glsl_compiler::Descriptor> *srd,const uint32_t type)
{
    const char *desc_type_name=vk_shader::descriptor_type_name[type];

    glsl_compiler::Descriptor *desc=srd->items;

    MaterialDescriptorResource *mdr;

    for(size_t i=0;i<srd->count;i++)
    {
        if(mdr_list.Get(desc->name,mdr))
        {
            mdr->shader_stage_bits|=ssb;
        }
        else
        {
            mdr=new MaterialDescriptorResource();

            mdr->shader_stage_bits=ssb;
            mdr->desc_type=type;
            mdr->desc=desc;
        }

        ++desc;
    }
}

void StatDescriptor(MDRList &mdr_list,ShaderDataInfo *sdi)
{
    ENUM_CLASS_FOR(vk_shader::DescriptorType,uint32_t,i)
    {
        StatDescriptorResource(mdr_list,sdi->shader_stage_bit,sdi->spv->resource+i,i);
    }
}
