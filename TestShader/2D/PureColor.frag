[Global]
[PerFrame]
[PerObject]
PureColorMaterial color_material;   //编译器会复制名为PureColorMaterial.ubo的文件到shader目录，并#include"PureColorMaterial.ubo"

[Const]
vec4 color=vec4(1,0,0);

[code]
void main()
{
    return color_material.Color;
}
