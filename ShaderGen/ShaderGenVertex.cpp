#include"ShaderGen.h"
#include<hgl/filesystem/FileSystem.h>

class ShaderGenVertex:public ShaderGen
{
public:

    ShaderGenVertex(ShaderDataManager *_sdm, const OSString &path):ShaderGen(_sdm, path) {}

private:

    void PreProcess() override
    {
        output_struct_name=sdm->GetStageName()+UTF8String("Output");

        output_struct_filename=output_struct_name+U8_TEXT(".glsl");
        
        AddInclude(output_struct_filename);
    }

    bool MakeOutput()
    {
        UTF8String codes;

        codes=  "#ifndef "+output_struct_name+"_GLSL\n"
                "#define "+output_struct_name+"_GLSL\n"
                "\n"
                "struct "+output_struct_name+"\n"
                "{\n";

        for(auto *ss:sdm->GetShaderStageIO().output)
            codes+="\t"+ss->type+"\t"+UTF8String(ss->name)+";\n";            

        codes+= "};\n"
                "#endif//"+output_struct_name+"_GLSL\n";

        const OSString fn=filesystem::MergeFilename(output_path, ToOSString(output_struct_filename));

        if(!filesystem::SaveMemoryToFile(fn, codes.c_str(),codes.Length()))
        {
            LOG_ERROR(OS_TEXT("Save file '")+fn+OS_TEXT("' failed."));
            return(false);
        }
        else
        {
            LOG_INFO(OS_TEXT("Save file '")+fn+OS_TEXT("' success."));
        }
        
        return(true);
    }

public:

    bool Begin() override
    {
        const auto &io=sdm->GetShaderStageIO();

        if(!io.input.IsEmpty())
        {
            for(auto *ss:io.input)
                Add("layout(location="+UTF8String::numberOf(ss->location)+") in "+ss->type+"\t"+UTF8String(ss->name)+"\n");

            AddLineBreak();
        }
        
        if(!io.output.IsEmpty())
        {
            if(!MakeOutput())
                return(false);

            AddStageOutput();
            AddLineBreak();
        }
        
        return(true);
    }

    bool End() override
    {
        return(true);
    }
};

ShaderGen *CreateShaderGenVertex(ShaderDataManager *sdm,const OSString &path)
{
    return(new ShaderGenVertex(sdm,path));
}