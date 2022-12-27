# Shader Generator

## Pixel Material Component文件

Pixel Material Component(以后简称PMC)用于描述每个象素的材质成份，用于前段的自定义材质输出，也用于最终的颜色计算。

    设计中包含SDR/HDR两种版本，以及Depth/Alpha等特殊版本。

    SDR对应的输出FBO一般为RGBA8,RGB565,RGB10A2等各种整数型，所以输出范围只能为0-1。

    HDR对应的输出FBO一般为RGBA16F,RGBA32F等各种float型，可以输出超出0-1的值。

范例：BlinnPhong.pmc
```glsl
//PMC结构用于保存一些最终计算的参数
#include"Light.glsl"
#include"Camera.glsl"

struct PMC
{
    vec3    Color;
    vec3    Normal;

    //以下为自动处理部分，需要则定义，不需要则无需定义
    float   Depth;
    vec3    WorldPosition;
};

//SDR最终输出计算函数`
vec4 SDRMain(in PMC pmc)
{
    vec3 rgb=SDRBlinnPhong(pmc);

    return vec4(rgb,1.0f);
}

//HDR最终输出计算函数
vec4 HDRMain(in PMC pmc){...}

//以下函数可以不存在，并且其输出值在SDRMain/HDRMain中仍然可以输出。

//自定义深度输出，该函数仅用于仅输出深度时的计算(比如OQ或ShadowMap)，正常颜色及光照计算时不使用该函数。
float DepthMain(out PMC pmc){...}

//自定义透明度输出，该函数仅用于仅输出透明度时的计算(比如OQ或ShadowMap、或Mask)，正常颜色及光照计算时不使用该函数。
float AlphaMain(out PMC pmc){...}
```

## 用户自定义部分
范例：BlinnPhong.mat
```glsl
#include"BlinnPhong.pmc"          //输出使用BlinnPhong.pmc

[Vertex Shader/Vertex Input]      //顶点Shader输入部分

vec3 Position;      //顶点坐标
vec3 Color;         //顶点颜色
vec3 Normal;        //顶点法线

[Vertex Shader/Instance Input]    //Instance输入部分
vec3 InstOffset;

[Vertex Shader/Output]            //顶点shader输出部分，会被定义为一个名为Output的结构，同时在下一段shader中是名为Input的结构
vec4 Color;
vec3 Normal;

[Vertex Shader]     //用户自定义顶点shader部分
void VSMain()
{
    Output.Color=Color;
    Output.Normal=Normal;
}

[Fragment Shader]   //用户自定义片元shader部分
void FSMain(inout PMC pmc)
{
    pmc.Color   =Input.Color;
    pmc.Normal  =Input.Normal;
}
```

## 标准Blinn-Phong前向渲染材质Shader生成范例

前向渲染Vertex shader部分
```glsl
//顶点输入部分，根据BlinnPhong.mat中的[Vertex Input]以及[Instance Input]部分生成
layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Color;
layout (location = 2) in vec3 Normal;

//vertex shader输出部分
layout (location = 0) out PMC Output;       //PMC结构由BlinnPhong.pmc提供

//主函数
void main()
{
    
}
```

前向渲染Fragment Shader部分
```glsl
//vertex shader输出，fragment shader输入部分
layout (location = 0) in PMC Input;         //PMC结构由BlinnPhong.pmc提供

layout (location = 0) out vec4 OuputColor;  //最终颜色输出

//主函数
void main()
{
    OutputColor=LDRMain(Input);             //LDRMain由BlinnPhong.lcs提供
}
```
