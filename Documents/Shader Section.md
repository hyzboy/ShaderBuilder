# Shader Section

## Input
该节通常用于定义vertex shader中的顶点数据输入流。

This section is used to specify the data stream for the vertex/instance input, usually located in a vertex shader.

## Output
该节用于定义shader的输出变量，同时Output节的内容会在下一个shader中以Input结构的名字出现。

This section is used to define the output variables of the shader, and the contents of the "Output" section will appear under the name of the "Input" structure in the next shader.

## Subpass
定义subpass输入成份信息
## Const
定义const变量
## Global
用于定义一些长期不会改变的内容，如太阳位置，屏幕大小等。
## PerFrame
定义一些每帧固定不会改变的内容，如摄像机信息等。
## PerMaterial
定义一些材质独有信息。
## PerObject
定义每个渲染对像的独有信息，典型如LocalToWorldMatrix。

## Instance
定义Instance渲染用数据

## Skeleton
定义骨骼动图用信息
