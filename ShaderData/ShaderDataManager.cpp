#include"ShaderDataManager.h"
/**
* �������ƻ�ȡһ��������
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
