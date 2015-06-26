//
//  ShaderManager.cpp
//  Base_on_glsl
//
//  Created by jimCheng on 15/3/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <string>
#include "ShaderManager.h"
#include "Memory.h"

using namespace std;

/* PUBLIC */

ShaderManager::ShaderManager() :
    initialized(false)
{
    // Do nothing
}

ShaderManager::~ShaderManager()
{
    release();
}

bool ShaderManager::init(const char* projectName)
{
    if (initialized)
    {
        return false;
    }
    
#define createShader(shaderType, vertexShaderFilePath, fragmentShaderFilePath) \
        shaders[shaderType] = Memory_NewHeapObject(Shader); \
        shaders[shaderType]->loadShadersFromFile(vertexShaderFilePath, fragmentShaderFilePath);
    
    string projectPath = string("/Users/jimCheng/projects/OpenglLibrary/") + projectName + "/Base/";
    createShader(SHADER_TYPE_IDENTITY, (projectPath + "identity.vsh").c_str(), (projectPath + "identity.fsh").c_str());
    createShader(SHADER_TYPE_IDENTITY_COLOR, (projectPath + "identity_color.vsh").c_str(), (projectPath + "identity_color.fsh").c_str());
    createShader(SHADER_TYPE_IDENTITY_COLOR_TEXTURE, (projectPath + "identity_color_texture.vsh").c_str(), (projectPath + "identity_color_texture.fsh").c_str());
    createShader(SHADER_TYPE_IDENTITY_TEXTURE, (projectPath + "identity_texture.vsh").c_str(), (projectPath + "identity_texture.fsh").c_str());
    createShader(SHADER_TYPE_MVP_TEXTURE_DIFFUSE, (projectPath + "mvp_texture_diffuse.vsh").c_str(), (projectPath + "mvp_texture_diffuse.fsh").c_str());
    createShader(SHADER_TYPE_MVP_COLOR, (projectPath + "mvp_color.vsh").c_str(), (projectPath + "mvp_color.fsh").c_str());
    
#undef createShader
    
    initialized = true;
    
    return true;
}

Shader* ShaderManager::shader(SHADER_TYPE shaderType)
{
    if (!initialized)
    {
        return nullptr;
    }
    
    return shaders[shaderType];
}

/* PRIVATE */

void ShaderManager::release()
{
    for (int i = 0; i < SHADER_TYPE_AMOUNT; ++i)
    {
        Memory_DestructHeapObject(shaders[i], Shader);
        shaders[i] = nullptr;
    }
}
