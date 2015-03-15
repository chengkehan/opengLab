//
//  Shader.h
//  Base_on_glsl
//
//  Created by jimCheng on 15/3/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __Base_on_glsl__Shader__
#define __Base_on_glsl__Shader__

#include "glHeader.h"

class Shader
{
public:
    Shader();
    ~Shader();
    
    bool loadShadersFromFile(const char* vertexShaderFile, const char* fragmentShaderFile);
    bool isValid();
    bool enable();
    bool disable();
    
    bool setFloat(const char* name, float x);
    bool setFloat4(const char* name, float x, float y, float z, float w);
    bool setFloat4x4(const char* name, float* float4x4);
    bool setTexture(const char* name, GLuint texture);
    
private:
    Shader(const Shader&);
    Shader operator=(const Shader&);
    
    void release();
    GLuint createShaderFromFile(const char* shaderFile, GLenum shaderType);
    
private:
    GLuint shaderProgram;
};

#endif /* defined(__Base_on_glsl__Shader__) */
