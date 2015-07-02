//
//  MemoryLeakDetector.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/22.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "MemoryLeakDetector.h"
#include "Memory.h"

/* PUBLIC */

MemoryLeakDetector::MemoryLeakDetector() :
    listDetailLines(true), ignoreUnimportantInfo(false)
{
    Memory::heap()->printRealtimeBytesInfo(true);
}

MemoryLeakDetector::MemoryLeakDetector(bool listDetailLines, bool ignoreUnimportantInfo, bool realtimeBytesInfo) :
    listDetailLines(listDetailLines), ignoreUnimportantInfo(ignoreUnimportantInfo)
{
    Memory::heap()->printRealtimeBytesInfo(realtimeBytesInfo);
}

MemoryLeakDetector::~MemoryLeakDetector()
{
    printf("Memory leak: %s\n", Memory::heap()->hasUnreleasedMemory() ? "true" : "false");
    printf("Bytes Used:%d, ReservedUnuned:%d\n", Memory::heap()->bytesUsed(), Memory::heap()->bytesReservedUnused());
    Memory::heap()->printLog(listDetailLines, ignoreUnimportantInfo);
}

/* PRIVATE */
