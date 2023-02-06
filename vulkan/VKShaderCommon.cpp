#include"VKShaderCommon.h"

namespace vk_shader
{
    const char *GetShaderStageName(const ShaderStageBits &type)
    {
        if(type==0)return(shader_stage_name_list[0]);

        if(type>ssbCallable)return(nullptr);

        uint32_t index=1;
        uint32_t value=(uint32_t)type;

        while(value>1)
        {
            ++index;
            value>>=1;
        }

        return shader_stage_name_list[index];
    }
}//namespace vk_shader
