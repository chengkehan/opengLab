//
//  ShaderManager.cpp
//  Base_on_cg
//
//  Created by jimCheng on 15/2/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "ShaderManager.h"
#include "cgUtil.h"

using namespace std;

/* PUBLIC */

ShaderManager::ShaderManager() :
    cgContext(nullptr), currentShader(nullptr)
{
    // Do nothing
}

ShaderManager::~ShaderManager()
{
    if (cgContext != nullptr)
    {
        cgDestroyContext(cgContext);
        cgContext = nullptr;
        
        for(int i = 0; i < SHADER_TYPE_AMOUNT; ++i)
        {
            delete shaders[i];
            shaders[i] = nullptr;
        }
        
        currentShader = nullptr;
    }
}

bool ShaderManager::init(const char* projectName)
{
    if (cgContext != nullptr)
    {
        return true;
    }
    
    cgContext = cgCreateContext();
    cgCheckError("create cg context", cgContext);
    cgGLSetDebugMode(CG_TRUE);
    cgSetParameterSettingMode(cgContext, CG_DEFERRED_PARAMETER_SETTING);
    
#define createShader(type, path) \
        shaders[type] = new Shader(#type); \
        if(!shaders[type]->init(cgContext, "main_vp", "main_fp")) \
        { \
            cgPrintThenExit("Initialize shader fail. " #type); \
        } \
        if(!shaders[type]->loadVertexAndFragmentProgramFromFile(path)) \
        { \
            cgPrintThenExit("Load shader fail. " #type); \
        }
    string path = string("/Users/jimCheng/projects/OpenglLibrary/") + projectName + "/Base";
    createShader(SHADER_TYPE_IDENTITY, (path + "/Shader_Identity.cg").c_str())
    createShader(SHADER_TYPE_FLAT, (path + "/Shader_Flat.cg").c_str())
    createShader(SHADER_TYPE_DIFFUSE, (path + "/Shader_Diffuse.cg").c_str())
    createShader(SHADER_TYPE_DIFFUSE_TEXTURE, (path + "/Shader_Diffuse_Texture.cg").c_str())
    createShader(SHADER_TYPE_TEXTURE, (path + "/Shader_Texture.cg").c_str())
#undef createShader
    
    return true;
}

Shader* ShaderManager::shader(enum SHADER_TYPE type)
{
    if (cgContext == nullptr)
    {
        cgPrintThenExit("cgContext is null");
        return nullptr;
    }
    
    return shaders[type];
}

Shader* ShaderManager::shader()
{
    if (currentShader == nullptr)
    {
        cgPrintThenExit("There is no shader in using");
    }
    
    return currentShader;
}

bool ShaderManager::useShader(enum SHADER_TYPE type)
{
    if (cgContext == nullptr)
    {
        cgPrintThenExit("cgContext is null");
        return false;
    }
    
    currentShader = shaders[type];
    return true;
}

/* PRIVATE */