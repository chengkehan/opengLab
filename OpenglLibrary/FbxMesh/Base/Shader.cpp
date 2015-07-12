//
//  Shader.cpp
//  Base_on_glsl
//
//  Created by jimCheng on 15/3/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "Shader.h"
#include "FileReader.h"
#include "util.h"
#include "glHeader.h"
#include "AttributeBindingLocation.h"
#include "Memory.h"

/* PUBLIC */

Shader::Shader() :
    shaderProgram(0)
{
    // Do nothing
}

Shader::~Shader()
{
    release();
}

bool Shader::loadShadersFromFile(const char *vertexShaderFile, const char *fragmentShaderFile)
{
    release();
    
    GLuint vertexShader = createShaderFromFile(vertexShaderFile, GL_VERTEX_SHADER);
    GLuint fragmentShader = createShaderFromFile(fragmentShaderFile, GL_FRAGMENT_SHADER);
    
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    
    glBindAttribLocation(shaderProgram, ATTRIBUTE_BINDING_LOCATION_POSITION, "vPosition");
    glBindAttribLocation(shaderProgram, ATTRIBUTE_BINDING_LOCATION_COLOR, "vColor");
    glBindAttribLocation(shaderProgram, ATTRIBUTE_BINDING_LOCATION_NORMAL, "vNormal");
    glBindAttribLocation(shaderProgram, ATTRIBUTE_BINDING_LOCATION_TEXTURE0, "vTexCoord0");
    
    glLinkProgram(shaderProgram);
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLint linkResult;
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linkResult);
    if (!linkResult)
    {
        GLint infoLength;
        glGetProgramiv(shaderProgram, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 1)
        {
            char* info = (char*)Memory_MallocHeapBlock(sizeof(char) * infoLength);
            glGetProgramInfoLog(shaderProgram, infoLength, nullptr, info);
            printThenExit("Link shader program error:%s. Vertes Shader:%s. Fragment Shader:%s.\n", info, vertexShaderFile, fragmentShaderFile);
            Memory_FreeHeapBlock(info);
        }
        release();
        return false;
    }
    
    return true;
}

bool Shader::isValid()
{
    return shaderProgram != 0;
}

bool Shader::enable()
{
    if (!isValid())
    {
        return false;
    }
    glUseProgram(shaderProgram);
    return true;
}

bool Shader::disable()
{
    glUseProgram(0);
    return true;
}

bool Shader::setFloat(const char *name, float x)
{
    if (!isValid())
    {
        return false;
    }
    
    GLint uniform = glGetUniformLocation(shaderProgram, name);
    glUniform1f(uniform, x);
    return true;
}

bool Shader::setFloat4(const char *name, float x, float y, float z, float w)
{
    if (!isValid())
    {
        return false;
    }
    
    GLuint uniform = glGetUniformLocation(shaderProgram, name);
    glUniform4f(uniform, x, y, z, w);
    return true;
}

bool Shader::setFloat4x4(const char *name, const float *float4x4)
{
    if (!isValid())
    {
        return false;
    }
    
    GLint uniform = glGetUniformLocation(shaderProgram, name);
    glUniformMatrix4fv(uniform, 1, GL_FALSE, float4x4);
    return true;
}

bool Shader::setTexture(const char *name, GLuint texture, GLint textureUnit)
{
    if (!isValid())
    {
        return false;
    }
    
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture);
    
    GLint uniform = glGetUniformLocation(shaderProgram, name);
    glUniform1i(uniform, textureUnit);
    return true;
}

/* PRIVATE */

void Shader::release()
{
    glDeleteProgram(shaderProgram);
    shaderProgram = 0;
}

GLuint Shader::createShaderFromFile(const char *shaderFile, GLenum shaderType)
{
    FileReader shaderFileReader;
    if(!shaderFileReader.read(shaderFile))
    {
        printThenExit("Read shader file fail. %s\n", shaderFile);
        return false;
    }

    GLuint shader = glCreateShader(shaderType);
    
    GLchar* shaderString[1];
    shaderString[0] = const_cast<GLchar*>(shaderFileReader.content());
    glShaderSource(shader, 1, shaderString, nullptr);
    
    glCompileShader(shader);

    GLint compileResult;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compileResult);
    if (!compileResult)
    {
        GLint infoLength;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLength);
        if (infoLength > 1)
        {
            char* info = (char*)Memory_MallocHeapBlock(sizeof(char) * infoLength);
            glGetShaderInfoLog(shader, infoLength, nullptr, info);
            printThenExit("Compile vertex shader error:%s. %s\n", info, shaderFile);
            Memory_FreeHeapBlock(info);
        }
        glDeleteShader(shader);
        printThenExit("Compile shader error.\n");
        return 0;
    }
    
    return shader;
}