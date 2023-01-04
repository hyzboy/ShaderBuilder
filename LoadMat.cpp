#include<hgl/type/LoadStringList.h>
#include"MatParse/MatSection.h"

using namespace hgl;

namespace
{
    class MatPMCParse:public MatSectionParse
    {
    public:

        const MatSection GetSection()const
        {
            return MatSection::PMC;
        }
        
    public:

        MatPMCParse(MatSource *ms) :MatSectionParse(ms)
        {}
        ~MatPMCParse()
        {}

        void Add(const UTF8String &str)
        {
            
        }
    };//class MatPMCParse
}//namespace

bool LoadMat(const OSString &filename)
{
    UTF8StringList sl;

    if(!LoadStringListFromTextFile(sl,filename))
    {
        LOG_ERROR(OS_TEXT("Load .Mat file failure. filename: ")+filename);
        return(false);
    }
    
    MatSection mat_section=MatSection::Unknow;

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
        }
        else
        {
            
            LOG_INFO(line);
        }
    }

    return(true);
}
