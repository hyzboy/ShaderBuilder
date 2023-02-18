#include"ShaderData/ShaderDataInfo.h"
#include<hgl/io/MemoryOutputStream.h>
#include<hgl/io/DataOutputStream.h>
#include"StatMaterialResource.h"

using namespace hgl;
using namespace hgl::io;

struct BinaryMaterial
{
    uint32_t shader_stage_bits;                 //所有shader的bit合集

    ObjectList<MemoryOutputStream> spv_block;   //所有spv数据的合集
};

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

bool MakeBinaryMaterial(ShaderMap &sm)
{
    BinaryMaterial bin_mtl;
    MDRList mdr_list;
    ShaderDataList sdl;

    sm.MakeValueList(sdl);

    {
        bin_mtl.shader_stage_bits=0;

        for(auto sdi:sdl)
        {
            bin_mtl.shader_stage_bits|=sdi->shader_stage_bit;

            bin_mtl.spv_block.Add(MakeSPVBlock(sdi));

            StatDescriptor(mdr_list,sdi);
        }
    }

    return(true);
}
