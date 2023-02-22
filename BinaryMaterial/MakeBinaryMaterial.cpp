#include"ShaderData/ShaderDataInfo.h"
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include<hgl/filesystem/FileSystem.h>
#include"StatMaterialResource.h"
#include"ShaderData/MaterialDescriptorManager.h"

using namespace hgl;
using namespace hgl::io;

using ShaderStageList=List<vk_shader::ShaderStageBits>;
using ShaderDataList=List<ShaderDataInfo *>;

void StatDescriptor(MDRList &mdr_list,ShaderDataInfo *sdi);

void WriteShaderStage(DataOutputStream *dos,const glsl_compiler::ShaderStageData &ssd,const char *hint)
{
    dos->WriteUint8(uint8(ssd.count));

    if(ssd.count<=0)return;

    const glsl_compiler::ShaderStage *ss=ssd.items;

    for(size_t i=0;i<ssd.count;i++)
    {
        dos->WriteUint8(ss->location);
        dos->WriteUint8(ss->basetype);
        dos->WriteUint8(ss->vec_size);
        dos->WriteAnsiTinyString(ss->name);

        ++ss;
    }
}

MemoryOutputStream *MakeSPVBlock(ShaderDataInfo *sdi)
{
    MemoryOutputStream *mos=new MemoryOutputStream;
    AutoDelete<DataOutputStream> dos=new LEDataOutputStream(mos);
    
    dos->WriteUint32(sdi->shader_stage_bit);
    dos->WriteUint32(sdi->spv->spv_length);
    dos->Write(sdi->spv->spv_data,sdi->spv->spv_length);

    WriteShaderStage(dos,sdi->spv->input,"in");
    WriteShaderStage(dos,sdi->spv->output,"out");

    return mos;
}

void WriteMDR(DataOutputStream *dos,const MaterialDescriptorResource *mdr)
{
    dos->WriteUint8((uint8)(mdr->desc_type));
    dos->WriteAnsiTinyString(mdr->desc->name);
    dos->WriteUint8(uint8(mdr->set_type));
    dos->WriteUint8(mdr->desc->set);
    dos->WriteUint8(mdr->desc->binding);
    dos->WriteUint32(mdr->shader_stage_bits);
}

void WriteMDR(DataOutputStream *dos,const MDRList &mdr_list)
{
    const uint count=mdr_list.GetCount();
    auto **it=mdr_list.GetDataList();

    dos->WriteUint8(count);

    if(count<=0)return;

    for(uint i=0;i<count;i++)
    {
        WriteMDR(dos,(*it)->right);

        ++it;
    }
}

bool MakeBinaryMaterial(const OSString &output_filename,ShaderMap &sm,const MaterialDescriptorManager *mdm)
{
    uint32_t shader_stage_bits;                 //所有shader的bit合集
    ObjectList<MemoryOutputStream> spv_block;   //所有spv数据的合集
    MDRList mdr_list;
    ShaderDataList sdl;

    sm.GetValueList(sdl);

    {
        shader_stage_bits=0;

        for(auto sdi:sdl)
        {
            shader_stage_bits|=sdi->shader_stage_bit;

            spv_block.Add(MakeSPVBlock(sdi));

            StatDescriptor(mdr_list,sdi);
        }
    }

    {
        auto *mdr=mdr_list.GetDataList();

        for(int i=0;i<mdr_list.GetCount();i++)
        {
            (*mdr)->right->set_type=mdm->GetSetType((*mdr)->left);
            ++mdr;
        }
    }

    {
        AutoDelete<MemoryOutputStream> mos=new MemoryOutputStream;
        AutoDelete<DataOutputStream> dos=new LEDataOutputStream(mos);

        {        
            constexpr char MATERIAL_FILE_HEADER[]=u8"Material\x1A";
            constexpr uint MATERIAL_FILE_HEADER_LENGTH=sizeof(MATERIAL_FILE_HEADER)-1;
            
            dos->Write(MATERIAL_FILE_HEADER,MATERIAL_FILE_HEADER_LENGTH);
            dos->WriteUint8(3);                                                      //version

            dos->WriteUint32(shader_stage_bits);
        }

        for(auto &ms:spv_block)
        {
            dos->WriteUint32(ms->GetSize());
            dos->Write(ms->GetData(),ms->GetSize());
        }

        WriteMDR(dos,mdr_list);

        if(!filesystem::SaveMemoryToFile(output_filename,mos->GetData(),mos->GetSize()))
        {
            LOG_ERROR(OS_TEXT("SaveMemoryToFile ")+output_filename+OS_TEXT(" failed!"));
            return(false);
        }
    }

    return(true);
}
