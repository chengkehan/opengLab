//
//  MemoryLeakDetector.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/22.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__MemoryLeakDetector__
#define __FbxMesh__MemoryLeakDetector__

class MemoryLeakDetector
{
public:
    MemoryLeakDetector();
    ~MemoryLeakDetector();
    
private:
    MemoryLeakDetector(const MemoryLeakDetector&);
    MemoryLeakDetector& operator=(const MemoryLeakDetector&);
};

#endif /* defined(__FbxMesh__MemoryLeakDetector__) */
