//
//  IHeap.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef FbxMesh_IHeap_h
#define FbxMesh_IHeap_h

class Heap
{
public:
    virtual void* allocateMemory(unsigned int numBytes) = 0;
    virtual void* allocateZeroMemory(unsigned int numBytes) = 0;
    virtual bool freeMemory(void* ptr) = 0;
    virtual void gc() = 0;
};

#endif
