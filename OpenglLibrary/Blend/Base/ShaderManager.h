//
//  ShaderManager.h
//  Base_on_cg
//
//  Created by jimCheng on 15/2/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __Base_on_cg__ShaderManager__
#define __Base_on_cg__ShaderManager__

#include <Cg/cg.h>
#include <Cg/cgGL.h>
#include "ShaderType.h"
#include "Shader.h"

class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();
    
    bool init(const char* projectName);
    Shader* shader(enum SHADER_TYPE type);
    Shader* shader();
    bool useShader(enum SHADER_TYPE type);
    
private:
    ShaderManager(const ShaderManager&);
    const ShaderManager& operator=(const ShaderManager&);
    
private:
    Shader* shaders[SHADER_TYPE_AMOUNT];
    Shader* currentShader;
    CGcontext cgContext;
};

#endif /* defined(__Base_on_cg__ShaderManager__) */
