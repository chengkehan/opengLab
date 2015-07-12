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

TinyHeap::TinyHeap() :
    realtimeBytesInfo(true)
{
    tinyMemory.init(4);
}

TinyHeap::~TinyHeap()
{
    // Do nothing
}

void* TinyHeap::allocateMemory(unsigned long numBytes)
{
    void* ptr = tinyMemory.allocateMemory(numBytes);
    printBytesInfo();
    return ptr;
}

void* TinyHeap::allocateMemory_debug(unsigned long numBytes, const char *file, unsigned int line)
{
    memoryLog.allocateMemoryLog(file, line);
    void* ptr = tinyMemory.allocateMemory(numBytes);
    printBytesInfo();
    return ptr;
}

void* TinyHeap::allocateZeroMemory(unsigned long numBytes)
{
    void* ptr = tinyMemory.allocateZeroMemory(numBytes);
    printBytesInfo();
    return ptr;
}

void* TinyHeap::allocateZeroMemory_debug(unsigned long numBytes, const char *file, unsigned int line)
{
    memoryLog.allocateMemoryLog(file, line);
    void* ptr = tinyMemory.allocateZeroMemory(numBytes);
    printBytesInfo();
    return ptr;
}

bool TinyHeap::freeMemory(void *ptr)
{
    bool result = tinyMemory.freeMemory(ptr);
    if(ptr != nullptr)
    {
        printBytesInfo();
    }
    return result;
}

bool TinyHeap::freeMemory_debug(void *ptr, const char *file, unsigned int line)
{
    bool result = tinyMemory.freeMemory(ptr);
    if(ptr != nullptr)
    {
        memoryLog.releaseMemoryLog(file, line);
        printBytesInfo();
    }
    return result;
}

void TinyHeap::gc()
{
    // Do nothing
}

bool TinyHeap::hasUnreleasedMemory()
{
    return tinyMemory.hasUnreleasedMemory();
}

void TinyHeap::printLog(bool listDetailLines, bool ignoreUnimportantInfo)
{
    memoryLog.printLog(listDetailLines, ignoreUnimportantInfo);
}

unsigned int TinyHeap::bytesUsed()
{
    return tinyMemory.bytesUsed();
}

unsigned int TinyHeap::bytesReservedUnused()
{
    return tinyMemory.bytesReservedUnused();
}

bool TinyHeap::printRealtimeBytesInfo(bool yesORno)
{
    realtimeBytesInfo = yesORno;
    return true;
}

/* PRIVATE */

void TinyHeap::printBytesInfo()
{
    if (realtimeBytesInfo)
    {
        printf("Heap Bytes Used:%d, ReservedUnuned:%d\n", tinyMemory.bytesUsed(), tinyMemory.bytesReservedUnused());
    }
}