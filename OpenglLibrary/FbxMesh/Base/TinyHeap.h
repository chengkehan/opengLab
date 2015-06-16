//
//  TinyHeap.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__TinyHeap__
#define __FbxMesh__TinyHeap__

#include "IHeap.h"
#include "TinyMemory.h"

class TinyHeap : public IHeap
{
public:
    TinyHeap();
    ~TinyHeap();
    
    virtual void* allocateMemory(unsigned int numBytes);
    virtual void* allocateZeroMemory(unsigned int numBytes);
    virtual bool freeMemory(void* ptr);
    virtual void gc();
    
private:
    TinyHeap(const TinyHeap&);
    TinyHeap& operator=(const TinyHeap&);
    
    jcgame::TinyMemory tinyMemory;
};

#endif /* defined(__FbxMesh__TinyHeap__) */
