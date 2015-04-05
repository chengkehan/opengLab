//
//  ReadShaderFileUtil.cpp
//  Base_cg
//
//  Created by jimCheng on 15/2/8.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "ReadShaderFileUtil.h"

ShaderFile::ShaderFile() :
    shaderProgramText(nullptr)
{
    // Do nothing
}

ShaderFile::~ShaderFile()
{
    releaseMemory();
}

bool ShaderFile::readFile(const char *filePath)
{
    if (filePath == nullptr)
    {
        return false;
    }
    
    releaseMemory();
    
    FILE* vpfile = fopen(filePath, "r");
    if (vpfile == NULL)
    {
        printf("File is not exists:%s.\n", filePath);
        return false;
    }
    fseek(vpfile, 0, SEEK_END);
    long vpFileSize = ftell(vpfile);
    rewind(vpfile);
    shaderProgramText = (char*)malloc(sizeof(char) * vpFileSize + 1);
    if (shaderProgramText == NULL)
    {
        printf("Allocate memory fail for reading file %s.\n", filePath);
        return false;
    }
    shaderProgramText[sizeof(char) * vpFileSize] = '\0';
    size_t vpFileResult = fread(shaderProgramText, 1, vpFileSize, vpfile);
    if (vpFileResult != vpFileSize)
    {
        printf("Read file fail:%s.\n", filePath);
        return false;
    }
    fclose(vpfile);
    return true;
}

void ShaderFile::releaseMemory()
{
    delete this->shaderProgramText;
    this->shaderProgramText = nullptr;
}
