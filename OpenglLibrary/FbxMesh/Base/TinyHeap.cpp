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

void* TinyHeap::allocateMemory_debug(unsigned int numBytes, const char *file, unsigned int line)
{
    memoryLog.allocateMemoryLog(file, line);
    return tinyMemory.allocateMemory(numBytes);
}

void* TinyHeap::allocateZeroMemory(unsigned int numBytes)
{
    return tinyMemory.allocateZeroMemory(numBytes);
}

void* TinyHeap::allocateZeroMemory_debug(unsigned int numBytes, const char *file, unsigned int line)
{
    memoryLog.allocateMemoryLog(file, line);
    return tinyMemory.allocateZeroMemory(numBytes);
}

bool TinyHeap::freeMemory(void *ptr)
{
    return tinyMemory.freeMemory(ptr);
}

bool TinyHeap::freeMemory_debug(void *ptr, const char *file, unsigned int line)
{
    memoryLog.releaseMemoryLog(file, line);
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

void TinyHeap::printLog(bool detail)
{
    memoryLog.printLog(detail);
}

/* PRIVATE */