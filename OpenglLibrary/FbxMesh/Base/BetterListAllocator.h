//
//  BetterListAllocator.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/17.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__BetterListAllocator__
#define __FbxMesh__BetterListAllocator__

class IBetterListAllocator
{
public:
    virtual void* allocate(unsigned int numBytes) = 0;
    virtual bool release(void* ptr) = 0;
};

class BetterListDefaultAllocator : public IBetterListAllocator
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

#endif /* defined(__FbxMesh__BetterListAllocator__) */
