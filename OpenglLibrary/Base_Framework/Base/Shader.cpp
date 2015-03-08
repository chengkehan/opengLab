//
//  Shader.cpp
//  Base_on_cg
//
//  Created by jimCheng on 15/2/15.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Shader.h"
#include "FileReader.h"
#include "cgUtil.h"

using namespace std;

/* PUBLIC */

Shader::Shader(const char* name) :
    cgContext(nullptr),
    cgVertexProgram(nullptr), cgFragmentProgram(nullptr)
{
    this->name = name;
}

Shader::~Shader()
{
    release();
}

bool Shader::init(CGcontext cgContext, const char *vertexProgramMainEntry, const char *fragmentProgramMainEntry)
{
    if (cgContext == nullptr)
    {
        cgPrintThenExit("Null input cgContext parameter");
        return false;
    }
    
    this->cgContext = cgContext;
    this->vertexProgramMainEntry = vertexProgramMainEntry;
    this->fragmentProgramMainEntry = fragmentProgramMainEntry;
    
    return true;
}

bool Shader::loadVertexProgramFromFile(const char *path)
{
    return loadProgramFromFile(path, true);
}

bool Shader::loadFragmentProgramFromFile(const char *path)
{
    return loadProgramFromFile(path, false);
}

bool Shader::loadVertexAndFragmentProgramFromFile(const char *path)
{
    return loadVertexProgramFromFile(path) && loadFragmentProgramFromFile(path);
}

bool Shader::loadVertexProgram(const char *program)
{
    return loadProgram(program, true);
}

bool Shader::loadFragmentProgram(const char *program)
{
    return loadProgram(program, false);
}

bool Shader::loadVertexAndFragmentProgram(const char *program)
{
    return loadVertexProgram(program) && loadFragmentProgram(program);
}

void Shader::enable()
{
    if (cgVertexProgram == nullptr)
    {
        cgPrint("Null vertex program. Cannot enable the vp shader\n");
    }
    if (cgFragmentProgram == nullptr)
    {
        cgPrint("Null fragment program. Cannot enable the fp shader\n");
    }
    
    glUseProgram(0);
    
    cgGLBindProgram(cgVertexProgram);
    cgCheckError("binding vertex program", cgContext);
    cgGLEnableProfile(cgVertexProfile);
    cgCheckError("enable vertex prifile", cgContext);
    cgGLBindProgram(cgFragmentProgram);
    cgCheckError("binding fragment program", cgContext);
    cgGLEnableProfile(cgFragmentProfile);
    cgCheckError("enable fragment profile", cgContext);
    
    for (enabledTextures_type_iterator iterator = enabledTextures.begin(); iterator != enabledTextures.end(); ++iterator)
    {
        EnabledTexture* item = *iterator;
        cgGLEnableTextureParameter(item->cgParameter);
        cgCheckError("cgGLEnableTextureParameter", cgContext);
    }
}

void Shader::disable()
{
    cgGLDisableProfile(cgVertexProfile);
    cgGLDisableProfile(cgFragmentProfile);
    glUseProgram(0);
}

void Shader::setFloat(const char *parameterName, float value)
{
    CGparameter cgParameter = getCGparameter(parameterName);
    if (cgParameter != nullptr)
    {
        cgSetParameter1f(cgParameter, value);
        cgCheckError("Shader::setFloat", cgContext);
    }
}

void Shader::setFloat2(const char *parameterName, float x, float y)
{
    CGparameter cgParameter = getCGparameter(parameterName);
    if (cgParameter != nullptr)
    {
        cgSetParameter2f(cgParameter, x, y);
        cgCheckError("Shader::setFloat2", cgContext);
    }
}

void Shader::setFloat4(const char *parameterName, float x, float y, float z, float w)
{
    CGparameter cgParameter = getCGparameter(parameterName);
    if (cgParameter != nullptr)
    {
        cgSetParameter4f(cgParameter, x, y, z, w);
        cgCheckError("Shader::setFloat4", cgContext);
    }
}

void Shader::setFloat4x4(const char *parameterName, const float *float4x4)
{
    CGparameter cgParameter = getCGparameter(parameterName);
    if (cgParameter != nullptr)
    {
        cgSetMatrixParameterfc(cgParameter, float4x4);
        cgCheckError("Shader::setFloat4x4", cgContext);
    }
}

void Shader::setTexture(const char *parameterName, GLuint texture)
{
    CGparameter cgParameter = getCGparameter(parameterName);
    if (cgParameter != nullptr)
    {
        cgGLSetTextureParameter(cgParameter, texture);
        cgCheckError("Shader::setTexture", cgContext);
        
        bool isSet = false;
        for (enabledTextures_type_iterator iterator = enabledTextures.begin(); iterator != enabledTextures.end(); ++iterator)
        {
            EnabledTexture* item = *iterator;
            if (item->parameterName == parameterName)
            {
                isSet = true;
                item->cgParameter = cgParameter;
                break;
            }
        }
        if (!isSet)
        {
            EnabledTexture* item = new EnabledTexture();
            item->parameterName = parameterName;
            item->cgParameter = cgParameter;
            enabledTextures.push_back(item);
        }
    }
}

/* PRIVATE */

void Shader::release()
{
    cgContext = nullptr;
    releaseVertexProgram();
    releaseFragmentProgram();
}

void Shader::releaseVertexProgram()
{
    if (cgVertexProgram != nullptr)
    {
        cgDestroyProgram(cgVertexProgram);
        cgVertexProgram = nullptr;
    }
}

void Shader::releaseFragmentProgram()
{
    if (cgFragmentProgram != nullptr)
    {
        cgDestroyProgram(cgFragmentProgram);
        cgFragmentProgram = nullptr;
    }
}

bool Shader::loadProgramFromFile(const char *path, bool vertexOrFragment)
{
    FileReader fileReader;
    if (!fileReader.read(path))
    {
        cgPrintThenExit("Read program file fail. %s\n", path);
        return false;
    }
    
    return loadProgram(fileReader.content(), vertexOrFragment);
}

bool Shader::loadProgram(const char *program, bool vertexOrFragment)
{
    if (cgContext == nullptr)
    {
        cgPrintThenExit("Null cgContext for program");
        return false;
    }
    
    if (program == nullptr)
    {
        cgPrintThenExit("Null input program parameter");
        return false;
    }
    
    // Profile
    CGprofile* cgProfile = vertexOrFragment ? &cgVertexProfile : &cgFragmentProfile;
    *cgProfile = cgGLGetLatestProfile(vertexOrFragment ? CG_GL_VERTEX : CG_GL_FRAGMENT);
    cgCheckError(("get latest profile " + name).c_str(), cgContext);
    cgGLSetOptimalOptions(*cgProfile);
    cgCheckError(("set optimal options " + name).c_str(), cgContext);
    
    // Program
    CGprogram* cgProgram = vertexOrFragment ? &cgVertexProgram : &cgFragmentProgram;
    const char* mainEntry = vertexOrFragment ? vertexProgramMainEntry.c_str() : fragmentProgramMainEntry.c_str();
    *cgProgram = cgCreateProgram(cgContext, CG_SOURCE, program, *cgProfile, mainEntry, nullptr);
    cgCheckError(("create program " + name).c_str(), cgContext);
    cgGLLoadProgram(*cgProgram);
    cgCheckError(("load program " + name).c_str(), cgContext);
    
    return true;
}

CGparameter Shader::getCGparameter(const char *parameterName)
{
    string parameterNameString(parameterName);
    cgParametersMapping_type_iterator findResult = cgParametersMapping.find(parameterNameString);
    if (findResult == cgParametersMapping.end())
    {
        CGparameter cgParameter = nullptr;
        
#define getNamedParameter(cgProgram, parameterName) \
        cgParameter = cgGetNamedParameter(cgProgram, parameterName); \
        if (cgParameter != nullptr) \
        { \
            cgParametersMapping.insert(cgParametersMapping_type_valuePair(parameterNameString, cgParameter)); \
        }
        
        getNamedParameter(cgVertexProgram, parameterName);
        if (cgParameter == nullptr)
        {
            getNamedParameter(cgFragmentProgram, parameterName);
        }
        
#undef getNamedParameter

        if (cgParameter == nullptr)
        {
            cgPrint("Cannot find the parameter. %s\n", parameterName);
        }
        
        return cgParameter;
    }
    else
    {
        return findResult->second;
    }
}