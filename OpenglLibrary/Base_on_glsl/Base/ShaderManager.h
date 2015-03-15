//
//  ShaderManager.h
//  Base_on_glsl
//
//  Created by jimCheng on 15/3/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __Base_on_glsl__ShaderManager__
#define __Base_on_glsl__ShaderManager__

#include "Shader.h"
#include "ShaderType.h"

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();
    
    bool init(const char* projectName);
    Shader* shader(SHADER_TYPE shaderType);
    
private:
    ShaderManager(const ShaderManager&);
    ShaderManager operator=(const ShaderManager&);
    
    void release();
    
private:
    Shader* shaders[SHADER_TYPE_AMOUNT];
    bool initialized;
};

#endif /* defined(__Base_on_glsl__ShaderManager__) */
