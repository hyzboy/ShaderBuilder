[input]
vec2 Position;      //顶点坐标
vec4 Color;         //顶点颜色

[output]            //output结构内的代码，会被整合成Output结构
vec4 Color;

[global]
CameraInfo camera;

[code]
vec4 VertexMain()
{
    Output.Color=Color;
    return vec4(Position,0,1);
}
