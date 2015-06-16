//
//  Memory.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/16.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__Memory__
#define __FbxMesh__Memory__

#include "TinyHeap.h"

class Memory
{
public:
    static IHeap* heap();
    
private:
    static IHeap* s_heap;
};

#endif /* defined(__FbxMesh__Memory__) */
