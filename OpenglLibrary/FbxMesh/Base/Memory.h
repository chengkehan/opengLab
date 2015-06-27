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
    static TinyHeap* heap();
    
private:
    static TinyHeap* s_heap;
};

template<class T>
inline void __Memory__InvokeDestructorManually(T* ptr)
{
    ((T*)ptr)->~T();
}

#ifdef __MEMORY_DEBUG__
    #define Memory_MallocHeapBlock(numBytes) Memory::heap()->allocateMemory_debug(numBytes, __FILE__, __LINE__)
#else
    #define Memory_MallocHeapBlock(numBytes) Memory::heap()->allocateMemory(numBytes)
#endif

#ifdef __MEMORY_DEBUG__
    #define Memory_FreeHeapBlock(ptr) Memory::heap()->freeMemory_debug(ptr);
#else
    #define Memory_FreeHeapBlock(ptr) Memory::heap()->freeMemory(ptr);
#endif

#ifdef __MEMORY_DEBUG__
    #define Memory_NewHeapObject(T, ...) new ((T*)Memory::heap()->allocateMemory_debug(sizeof(T), __FILE__, __LINE__))T(__VA_ARGS__)
#else
    #define Memory_NewHeapObject(T, ...) new ((T*)Memory::heap()->allocateMemory(sizeof(T)))T(__VA_ARGS__)
#endif

#ifdef __MEMORY_DEBUG__
    #define Memory_DeleteHeapObject(ptr) __Memory__InvokeDestructorManually(ptr); Memory::heap()->freeMemory_debug(ptr, __FILE__, __LINE__);
#else
    #define Memory_DeleteHeapObject(ptr) __Memory__InvokeDestructorManually(ptr); Memory::heap()->freeMemory(ptr);
#endif


#endif /* defined(__FbxMesh__Memory__) */
