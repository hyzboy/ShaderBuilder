cmake_minimum_required(VERSION 3.0)

project(ShaderBuilder)

add_definitions(-DUNICODE -D_UNICODE)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

SET(SHADER_BUILDER_LIBRARY  CMCore
                            CMPlatform
                            CMUtil
                            CMSceneGraph
                            ${HGL_MATH_LIB})

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

SET(SHADER_DATA_SOURCE  ShaderData/pmc.h
                        ShaderData/Varying.h
                        ShaderData/ShaderDescriptorManager.h
                        ShaderData/ShaderDescriptorManager.cpp
                        ShaderData/ShaderDataInfo.h
                        ShaderData/ShaderDataInfo.cpp
                        ShaderData/ResortShader.cpp
                        ShaderData/MaterialDataInfo.h
                        ShaderData/MaterialDescriptorManager.h
                        ShaderData/MaterialDescriptorManager.cpp)

SOURCE_GROUP("Shader Data" FILES ${SHADER_DATA_SOURCE})

SET(SHADER_PARSE_SOURCE ShaderParse/ShaderSection.h
                        ShaderParse/ShaderSection.cpp
                        ShaderParse/ShaderSectionParse.h
                        ShaderParse/ShaderSectionParse.cpp
                        ShaderParse/SSP_Code.cpp
                        ShaderParse/SSP_Const.cpp
                        ShaderParse/SSP_Varying.cpp
                        ShaderParse/SSP_Data.cpp
)

SOURCE_GROUP("Shader Parse" FILES ${SHADER_PARSE_SOURCE})

SET(SHADER_LOAD_SOURCE  ShaderGen/LoadMat.cpp
                        ShaderGen/LoadShader.cpp
                        ShaderGen/LoadUBO.cpp)

SOURCE_GROUP("Shader Generate" FILES ${SHADER_LOAD_SOURCE})

SET(SHADER_GEN_SOURCE   ShaderGen/ShaderGen.h
                        ShaderGen/ShaderGen.cpp
                        ShaderGen/ShaderGenVertex.cpp
                        ShaderGen/ShaderGenGeometry.cpp
                        ShaderGen/ShaderGenFragment.cpp)

SOURCE_GROUP("Shader Generate\\Gen" FILES ${SHADER_GEN_SOURCE})

SET(BINARY_MATERIAL_SOURCE  BinaryMaterial/CompileToSPV.cpp
                            BinaryMaterial/StatMaterialResource.cpp
                            BinaryMaterial/StatMaterialResource.h
                            BinaryMaterial/MakeBinaryMaterial.cpp)

SOURCE_GROUP("Binary Material" FILES ${BINARY_MATERIAL_SOURCE})

macro(shader_builder_project project_name)
    target_link_libraries(${project_name} PRIVATE ${SHADER_BUILDER_LIBRARY})

    if(UNIX)
        target_link_libraries(${project_name} PRIVATE dl)
    endif()

    IF(WIN32)
        set_debugger_directory(${project_name} ${CMAKE_CURRENT_SOURCE_DIR})

        target_sources(${project_name} PRIVATE ${CM_MANIFEST})
    ENDIF()

    set_property(TARGET ${project_name} PROPERTY FOLDER "CM/Tools/ShaderBuilder")
endmacro()
                               
add_executable(ShaderBuilder    ShaderBuilder.cpp
                                Config.cpp
                                ${BINARY_MATERIAL_SOURCE}
                                ${SHADER_LOAD_SOURCE}
                                ${SHADER_GEN_SOURCE}
                                ${SHADER_GEN_MANAGER_SOURCE}
                                ${SHADER_DATA_SOURCE}
                                ${SHADER_PARSE_SOURCE}
                                ${GLSL_Parse_SOURCE}
                                ${GLSL_COMPILER_SOURCE}
                                ${VK_SHADER_COMMON_SOURCE})
                                
shader_builder_project(ShaderBuilder)