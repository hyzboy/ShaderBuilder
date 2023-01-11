[Global]
[PerFrame]
[PerObject]
PureColorMaterial color_material;   //编译器会复制名为PureColorMaterial.ubo的文件到shader目录，并#include"PureColorMaterial.ubo"

[main]
vec4 main()
{
    return color_material.Color;
}
