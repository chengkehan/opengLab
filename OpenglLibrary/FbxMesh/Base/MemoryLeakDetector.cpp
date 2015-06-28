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

MemoryLeakDetector::MemoryLeakDetector()
{
    // Do nothing
}

MemoryLeakDetector::~MemoryLeakDetector()
{
    printf("Memory leak: %s\n", Memory::heap()->hasUnreleasedMemory() ? "true" : "false");
    
#ifdef __MEMORY_DEBUG_DETAIL__
    Memory::heap()->printLog(true);
#else
    Memory::heap()->printLog(false);
#endif
}

/* PRIVATE */
