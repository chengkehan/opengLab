//
//  TinyHeap.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__TinyHeap__
#define __FbxMesh__TinyHeap__

#include "TinyMemory.h"
#include "MemoryLog.h"

class TinyHeap
{
public:
    TinyHeap();
    ~TinyHeap();
    
    void* allocateMemory(unsigned int numBytes);
    void* allocateMemory_debug(unsigned int numBytes, const char* file, unsigned int line);
    void* allocateZeroMemory(unsigned int numBytes);
    void* allocateZeroMemory_debug(unsigned int numBytes, const char* file, unsigned int line);
    bool freeMemory(void* ptr);
    bool freeMemory_debug(void* ptr, const char* file, unsigned int line);
    void gc();
    bool hasUnreleasedMemory();
    void printLog(bool listDetailLines, bool ignoreUnimportantInfo);
    unsigned int bytesUsed();
    unsigned int bytesReservedUnused();
    bool printRealtimeBytesInfo(bool yesORno);
    
private:
    TinyHeap(const TinyHeap&);
    TinyHeap& operator=(const TinyHeap&);
    
    jcgame::TinyMemory tinyMemory;
    MemoryLog memoryLog;
    bool realtimeBytesInfo;
    
    void printBytesInfo();
};

#endif /* defined(__FbxMesh__TinyHeap__) */
