//
//  FbxAnimationStack.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/7/12.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxAnimationStack.h"
#include "Memory.h"

/* PUBLIC */

FbxAnimationStack::FbxAnimationStack() :
    name(nullptr)
{
    // Do nothing
}

FbxAnimationStack::~FbxAnimationStack()
{
    releaseName();
}

bool FbxAnimationStack::setName(const char *name)
{
    releaseName();
    
    if (name == nullptr)
    {
        return true;
    }
    
    size_t strLength = strlen(name);
    this->name = (char*)Memory_MallocHeapBlock(sizeof(char) * (strLength + 1));
    memcpy(this->name, name, strLength);
    this->name[strLength] = '\0';
    
    return true;
}

const char* FbxAnimationStack::getName()
{
    return name;
}

/* PRIVATE */

void FbxAnimationStack::releaseName()
{
    Memory_FreeHeapBlock(name);
    name = nullptr;
}
