//
//  TinyHeap.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "TinyHeap.h"
#include <stdio.h>

/* PUBLIC */

TinyHeap::TinyHeap()
{
    tinyMemory.init(4);
}

TinyHeap::~TinyHeap()
{
    // Do nothing
}

void* TinyHeap::allocateMemory(unsigned int numBytes)
{
    return tinyMemory.allocateMemory(numBytes);
}

void* TinyHeap::allocateZeroMemory(unsigned int numBytes)
{
    return tinyMemory.allocateZeroMemory(numBytes);
}

bool TinyHeap::freeMemory(void *ptr)
{
    return tinyMemory.freeMemory(ptr);
}

void TinyHeap::gc()
{
    // Do nothing
}

bool TinyHeap::hasUnreleasedMemory()
{
    return tinyMemory.hasUnreleasedMemory();
}

/* PRIVATE */