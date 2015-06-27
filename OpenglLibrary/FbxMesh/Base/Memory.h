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

template<class T>
inline void __Memory__InvokeDestructorManually(T* ptr)
{
    ((T*)ptr)->~T();
}

#define Memory_MallocHeapBlock(numBytes) Memory::heap()->allocateMemory(numBytes)

#define Memory_FreeHeapBlock(ptr) Memory::heap()->freeMemory(ptr);

#define Memory_NewHeapObject(T, ...) new ((T*)Memory::heap()->allocateMemory(sizeof(T)))T(__VA_ARGS__)

#define Memory_DeleteHeapObject(ptr) __Memory__InvokeDestructorManually(ptr); Memory::heap()->freeMemory(ptr);

#endif /* defined(__FbxMesh__Memory__) */
