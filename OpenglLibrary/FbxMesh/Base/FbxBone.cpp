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
    name(nullptr), indices(nullptr), weights(nullptr)
{
    m3dLoadIdentity44(bindpose);
}

FbxBone::~FbxBone()
{
    releaseName();
    releaseIndices();
    releaseWeights();
}

bool FbxBone::setName(const char *name)
{
    releaseName();
    
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

bool FbxBone::setIndices(const int *indices, unsigned int numIndices)
{
    if (numIndices != this->numIndices)
    {
        return false;
    }
    
    releaseIndices();
    
    if (indices == nullptr)
    {
        return true;
    }
    
    this->indices = (int*)Memory_MallocHeapBlock(sizeof(int) * numIndices);
    memcpy(this->indices, indices, sizeof(int) * numIndices);
    
    return true;
}

const int* FbxBone::getIndices()
{
    return indices;
}

bool FbxBone::setWeights(const double *weights, unsigned int numWeights)
{
    if (numWeights != this->numIndices)
    {
        return false;
    }
    
    releaseWeights();
    
    if (weights == nullptr)
    {
        return true;
    }
    
    this->weights = (double*)Memory_MallocHeapBlock(sizeof(double) * numIndices);
    memcpy(this->weights, weights, sizeof(double) * numIndices);
    
    return true;
}

const double* FbxBone::getWeights()
{
    return weights;
}

bool FbxBone::setNumIndices(unsigned int numIndices)
{
    this->numIndices = numIndices;
    return true;
}

unsigned int FbxBone::getNumIndices()
{
    return numIndices;
}

bool FbxBone::setBindpose(const M3DMatrix44f& bindpose)
{
    memcpy(this->bindpose, bindpose, sizeof(M3DMatrix44f));
    return true;
}

const M3DMatrix44f& FbxBone::getBindpose()
{
    return bindpose;
}

/* PRIVATE */

void FbxBone::releaseName()
{
    Memory_FreeHeapBlock(name);
    name = nullptr;
}

void FbxBone::releaseIndices()
{
    Memory_FreeHeapBlock(indices);
    indices = nullptr;
}

void FbxBone::releaseWeights()
{
    Memory_FreeHeapBlock(weights);
    weights = nullptr;
}
