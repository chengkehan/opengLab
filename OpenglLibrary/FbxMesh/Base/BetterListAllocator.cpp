//
//  BetterListAllocator.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/17.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "BetterListAllocator.h"
#include <stdlib.h>

/* BetterListDefaultAllocator PUBLIC */

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

/* BetterListDefaultAllocator PRIVATE */
