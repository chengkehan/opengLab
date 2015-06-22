//
//  Memory.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__Memory__
#define __FbxMesh__Memory__

#include <iostream>
#include "TinyHeap.h"

class Memory
{
public:
    Memory();
    ~Memory();
    static Heap* heap();
    
private:
    static Heap* s_heap;
};

//#define Memory_NewHeapObject(ptr, T, ...) ptr = (T*)Memory::heap()->allocateMemory(sizeof(T)); \
//                                            new (ptr)T(__VA_ARGS__);
#define Memory_NewHeapObject(T, ...) new ((T*)Memory::heap()->allocateMemory(sizeof(T)))T(__VA_ARGS__)

#define Memory_DeleteHeapObject(ptr) Memory::heap()->freeMemory(ptr); ptr = nullptr;

#endif /* defined(__FbxMesh__Memory__) */
