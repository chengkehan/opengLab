//
//  BetterListHeapMemoryAllocator.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/20.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__BetterListHeapMemoryAllocator__
#define __FbxMesh__BetterListHeapMemoryAllocator__

#include "BetterListAllocator.h"

class BetterListHeapMemoryAllocator : public BetterListAllocator
{
public:
    BetterListHeapMemoryAllocator();
    ~BetterListHeapMemoryAllocator();
    
    virtual void* allocate(unsigned int numBytes);
    virtual bool release(void* ptr);
    
private:
    BetterListHeapMemoryAllocator(const BetterListHeapMemoryAllocator&);
    BetterListHeapMemoryAllocator& operator=(const BetterListHeapMemoryAllocator&);
};

#endif /* defined(__FbxMesh__BetterListHeapMemoryAllocator__) */
