//
//  ReadShaderFileUtil.h
//  Base_cg
//
//  Created by jimCheng on 15/2/8.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef Base_cg_ReadShaderFileUtil_h
#define Base_cg_ReadShaderFileUtil_h

class ShaderFile
{
public:
    char* shaderProgramText;
    
public:
    ShaderFile();
    ~ShaderFile();
    
public:
    bool readFile(const char* filePath);
    
private:
    ShaderFile(const char* shaderProgramText);
    ShaderFile(const ShaderFile& shaderFile);
    
private:
    void releaseMemory();
};

#endif
