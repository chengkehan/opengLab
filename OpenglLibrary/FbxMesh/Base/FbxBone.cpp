//
//  FbxBone.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxBone.h"
#include <string.h>
#include "Memory.h"

/* PUBLIC */

FbxBone::FbxBone() :
    name(nullptr)
{
    // Do nothing
}

FbxBone::~FbxBone()
{
    Memory_FreeHeapBlock(name);
    name = nullptr;
}

bool FbxBone::setName(const char *name)
{
    Memory_FreeHeapBlock(this->name);
    this->name = nullptr;
    
    if (name == nullptr)
    {
        return true;
    }
    
    size_t length = strlen(name);
    this->name = (char*)Memory_MallocHeapBlock((unsigned int)(length + 1));
    memcpy(this->name, name, length);
    this->name[length] = '\0';
    
    return true;
}

const char* FbxBone::getName()
{
    return name;
}

bool FbxBone::addChild(FbxBone *bone)
{
    if (bone == nullptr)
    {
        return false;
    }
    
    return childrenBones.add(bone);
}

FbxBone* FbxBone::getChild(unsigned int index)
{
    if (index >= childrenBones.length())
    {
        return nullptr;
    }
    
    return childrenBones[index];
}

unsigned int FbxBone::numChildren()
{
    return childrenBones.length();
}

/* PRIVATE */
