cmake_minimum_required(VERSION 3.0)

project(ShaderBuilder)

add_definitions(-DUNICODE -D_UNICODE)

SET(SHADER_COMPILER_LIBRARY CMCore
                            CMPlatform
                            CMUtil
                            CMSceneGraph)

SET(GLSL_COMPILER_SOURCE	GLSLCompiler/GLSLCompiler.h
                            GLSLCompiler/GLSLCompiler.cpp)
SOURCE_GROUP("GLSL Compiler" FILES ${GLSL_COMPILER_SOURCE})

SET(VK_SHADER_COMMON_SOURCE vulkan/VKShaderCommon.h
                            vulkan/VKShaderCommon.cpp)
SOURCE_GROUP("Vulkan Shader Common" FILES ${VK_SHADER_COMMON_SOURCE})

add_executable(ShaderBuilder ShaderBuilder.cpp ${GLSL_COMPILER_SOURCE} ${VK_SHADER_COMMON_SOURCE})

set_property(TARGET ShaderBuilder PROPERTY FOLDER "CM/Tools/ShaderBuilder")