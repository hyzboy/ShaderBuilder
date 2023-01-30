#include"ShaderDataManager.h"
/**
* 根据名称获取一个描述符
*/
ShaderDescriptor *ShaderDataManager::GetDescriptor(const char *name)
{
    if(!name||!*name)
        return(nullptr);

    for(ShaderDescriptor *sd:descriptor_list)
    {
        if(strcmp(sd->name,name)==0)
            return sd;
    }

    return(nullptr);
}
