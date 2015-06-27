//
//  FbxBone.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxBone.h"
#include <string.h>

unsigned int FbxBone::MAX_CHARS_OF_NAME = 20;
unsigned int FbxBone::MAX_CHILDREN_BONES_AMOUNT = 20;

/* PUBLIC */

FbxBone::FbxBone() :
    numChildrenBones(0)
{
    emptyName();
}

FbxBone::~FbxBone()
{
    emptyName();
    numChildrenBones = 0;
}

bool FbxBone::setName(const char *name)
{
    if (name == nullptr)
    {
        emptyName();
        return true;
    }
    
    size_t length = strlen(name);
    
    if(length == 0)
    {
        emptyName();
        return true;
    }
    
    if(length > FbxBone::MAX_CHARS_OF_NAME)
    {
        printf("Limited max chars of bone's name.\n");
        return false;
    }
    
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
    
    if (numChildrenBones >= FbxBone::MAX_CHILDREN_BONES_AMOUNT)
    {
        printf("Limited max amount of bones' children.\n");
        return false;
    }
    
    childrenBones[numChildrenBones] = bone;
    ++numChildrenBones;
    return true;
}

FbxBone* FbxBone::getChild(unsigned int index)
{
    if (index >= FbxBone::MAX_CHILDREN_BONES_AMOUNT)
    {
        return nullptr;
    }
    
    return childrenBones[index];
}

unsigned int FbxBone::numChildren()
{
    return numChildrenBones;
}

/* PRIVATE */

void FbxBone::emptyName()
{
    name[0] = '\0';
}