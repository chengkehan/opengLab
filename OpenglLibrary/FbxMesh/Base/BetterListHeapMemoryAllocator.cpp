//
//  BetterListHeapMemoryAllocator.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/20.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "BetterListHeapMemoryAllocator.h"
#include "Memory.h"

/* PUBLIC */

BetterListHeapMemoryAllocator::BetterListHeapMemoryAllocator()
{
    // Do nothing
}

BetterListHeapMemoryAllocator::~BetterListHeapMemoryAllocator()
{
    // Do nothing
}

void* BetterListHeapMemoryAllocator::allocate(unsigned int numBytes)
{
    return Memory::heap()->allocateMemory(numBytes);
}

bool BetterListHeapMemoryAllocator::release(void *ptr)
{
    return Memory::heap()->freeMemory(ptr);
}

/* PRIVATE */
