//
//  BetterListDefaultAllocator.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/20.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "BetterListDefaultAllocator.h"
#include <stdlib.h>

/* PUBLIC */
BetterListDefaultAllocator::BetterListDefaultAllocator()
{
    
}

BetterListDefaultAllocator::~BetterListDefaultAllocator()
{
    
}

void* BetterListDefaultAllocator::allocate(unsigned int numBytes)
{
    return malloc(numBytes);
}

bool BetterListDefaultAllocator::release(void *ptr)
{
    free(ptr);
    return true;
}

/* PRIVATE */