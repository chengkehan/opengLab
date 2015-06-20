//
//  BetterListAllocator.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/17.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__BetterListAllocator__
#define __FbxMesh__BetterListAllocator__

class BetterListAllocator
{
public:
    virtual void* allocate(unsigned int numBytes) = 0;
    virtual bool release(void* ptr) = 0;
};

#endif /* defined(__FbxMesh__BetterListAllocator__) */
