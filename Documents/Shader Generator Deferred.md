## GBuffer Format(以后简称GBF)用于描述GBuffer格式。

GBuffer Format用于在延迟渲染中描述GBuffer格式，由于GBuffer可能是压缩格式，所以与PMC描述可能一样，也有可能不一定。并且会提供与PMC的互相转换代码。如果不使用延迟渲染，则不需要GBF文件

范例：BlinnPhong.gbf
```c++
#include"BlinnPhong.pmc"            //使用BlinnPhong.PMC
#include"SpheremapNormal.glsl";     //使用Spheremap Normal压缩解压缩算法

struct GBuffer                      //GBuffer描述结构
{
    vec3 Color;
    vec2 NormalXY;
};

void GB2PMC(inout PMC pmc,in GBuffer gb)
{
    pmc.Color           =gb.Color;
    pmc.Normal          =Normal2to3(gb.NormalXY);
}

void PMC2GB(inout GBuffer gb,in PMC pmc)
{
    gb.Color.rgb        =pmc.Color;
    gb.NormalXY.xy      =Normal3to2(pmc.Normal);
}
```

## 标准Blinn-Phong延迟渲染材质Shader生成范例

延迟渲染GBuffer生成阶段Vertex Shader部分
```glsl
//顶点输入部分，根据BlinnPhong.mat中的[Vertex Input]以及[Instance Input]部分生成
layout (location = 0) in vec3 VI_Position;
layout (location = 1) in vec3 VI_Color;
layout (location = 2) in vec3 VI_Normal;

//vertex shader输出部分
layout (location = 0) out PMC Output;       //PMC结构由BlinnPhong.pmc提供

//主函数
void main()
{
    VIC2PMC(Output);                        //VIC2PMC由BlinnPhong.2pm提供
}
```

延迟渲染GBuffer生成阶段Fragment Shader部分
```glsl
//vertex shader输出，fragment shader输入部分
layout (location = 0) in PMC Input;         //PMC结构由BlinnPhong.pmc提供

//GBuffer输出部分，根据BlinnPhong.gbf生成
layout (location = 0) out vec3 GB_Color;
layout (location = 1) out vec2 GB_NormalXY;

void main()
{
    PMC2GB(Input);                          //PMC2GB函数由BlinnPhong.gbf提供
}
```

延迟渲染合成阶段Fragment Shader部分
```glsl
//GBuffer输入部分，根据BlinnPhong.gbf生成
sampler2D GB_Color;
sampler2D GB_NormalXY;

uniform PMC pmc;                            //PMC结构由BlinnPhong.pmc提供

layout (location = 0) out vec4 OuputColor;  //最终颜色输出

void main()
{
    GB2PMC(pmc)                             //从GBuffer中解释出PMC数据，该函数由BlinnPhong.gbf提供

    OutputColor=LDRMain(pmc);               //计算最终颜色输出，LDRMain由BlinnPhong.lcs提供
}
```