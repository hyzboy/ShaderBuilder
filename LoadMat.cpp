#include<hgl/type/LoadStringList.h>
#include"MatParse/MatSection.h"
#include"MatParse/MatSectionParse.h"

using namespace hgl;

bool LoadMat(const OSString &filename)
{
    UTF8StringList sl;

    if(!LoadStringListFromTextFile(sl,filename))
    {
        LOG_ERROR(OS_TEXT("Load .Mat file failure. filename: ")+filename);
        return(false);
    }
    
    MatSection mat_section=MatSection::Unknow;
    MatSectionParse *msp=nullptr;

    for(uint i = 0;i < sl.GetCount();i++)
    {
        UTF8String raw_line=sl[i];
        UTF8String line;

        if(raw_line.IsEmpty())continue;

        //清除单行注释
        {
            int comment_pos;
            comment_pos=raw_line.FindString("//");

            if(comment_pos!=-1)
            {   
                line=raw_line; 
                line.ClipLeft(comment_pos);
            }
            else
            {
                line=raw_line;
            }
        }

        line=line.Trim();        //清除左右两端不可视字符

        if(line.IsEmpty())continue;

        if(line.GetFirstChar()=='['
         &&line.GetLastChar()==']')
        {
            line=line.SubString(1,line.Length()-2);
                    
            LOG_INFO("section: "+line);

            mat_section = ParseMatSection(line);

            SAFE_CLEAR(msp)

            msp=CreateMSP(mat_section);
        }
        else
        {
            if(msp)
            {
                msp->Add(line,raw_line);
            }
            else
            {
                LOG_INFO(line);
            }
        }
    }

    SAFE_CLEAR(msp);
    return(true);
}
