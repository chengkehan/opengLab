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
    // Do nothing
}

MemoryLeakDetector::MemoryLeakDetector(bool listDetailLines, bool ignoreUnimportantInfo) :
    listDetailLines(listDetailLines), ignoreUnimportantInfo(ignoreUnimportantInfo)
{
    // Do nothing
}

MemoryLeakDetector::~MemoryLeakDetector()
{
    printf("Memory leak: %s\n", Memory::heap()->hasUnreleasedMemory() ? "true" : "false");
    printf("MemoryLeakDetector:listDetailLines(%s),ignoreUnimportantInfo(%s)\n", listDetailLines ? "true" : "false", ignoreUnimportantInfo ? "true" : "false");
    Memory::heap()->printLog(listDetailLines, ignoreUnimportantInfo);
}

/* PRIVATE */
