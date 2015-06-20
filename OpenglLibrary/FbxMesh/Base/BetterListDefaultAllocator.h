//
//  BetterListDefaultAllocator.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/20.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__BetterListDefaultAllocator__
#define __FbxMesh__BetterListDefaultAllocator__

#include "BetterListAllocator.h"

class BetterListDefaultAllocator : public BetterListAllocator
{
public:
    BetterListDefaultAllocator();
    ~BetterListDefaultAllocator();
    
    virtual void* allocate(unsigned int numBytes);
    virtual bool release(void* ptr);
    
private:
    BetterListDefaultAllocator(const BetterListDefaultAllocator&);
    BetterListDefaultAllocator& operator=(const BetterListDefaultAllocator&);
};

#endif /* defined(__FbxMesh__BetterListDefaultAllocator__) */
