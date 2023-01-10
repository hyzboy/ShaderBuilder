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


SET(GLSL_Parse_SOURCE   GLSLParse/GLSLToken.h
                        GLSLParse/GLSLTokenizer.h
                        GLSLParse/GLSLTokenizer.cpp)
#                        GLSLParse/GLSLParse.cpp)
source_group("GLSL Parse" FILES ${GLSL_Parse_SOURCE})

SET(VK_SHADER_COMMON_SOURCE vulkan/VKShaderCommon.h
                            vulkan/VKShaderCommon.cpp)
SOURCE_GROUP("Vulkan Shader Common" FILES ${VK_SHADER_COMMON_SOURCE})

SET(SHADER_PARSE_SOURCE ShaderParse/ShaderSection.h
                        ShaderParse/ShaderSection.cpp
                        ShaderParse/ShaderSectionParse.h
                        ShaderParse/ShaderSectionParse.cpp
                        ShaderParse/SSP_Code.cpp
                        ShaderParse/SSP_Varying.cpp
)

SOURCE_GROUP("ShaderParse" FILES ${SHADER_PARSE_SOURCE})

add_executable(ShaderBuilder    ShaderBuilder.cpp 
                                LoadMat.cpp
                                ${SHADER_PARSE_SOURCE}
                                ${GLSL_Parse_SOURCE}
                                ${GLSL_COMPILER_SOURCE}
                                ${VK_SHADER_COMMON_SOURCE})

set_property(TARGET ShaderBuilder PROPERTY FOLDER "CM/Tools/ShaderBuilder")

target_link_libraries(ShaderBuilder PRIVATE CMCore
                                            CMPlatform
                                            CMUtil
                                            CMSceneGraph)

IF(WIN32)
    set_debugger_directory(ShaderBuilder ${CMAKE_CURRENT_SOURCE_DIR})
ENDIF()
