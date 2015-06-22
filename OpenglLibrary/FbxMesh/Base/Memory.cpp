//
//  Memory.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "Memory.h"

/* STATIC */

Heap* Memory::s_heap = nullptr;

Heap* Memory::heap()
{
    if (s_heap == nullptr)
    {
        s_heap = new TinyHeap();
    }
    return s_heap;
}

/* PUBLIC */

Memory::Memory()
{
    // Do nothing
}

Memory::~Memory()
{
    // Do nothing
}


/* PRIVATE */