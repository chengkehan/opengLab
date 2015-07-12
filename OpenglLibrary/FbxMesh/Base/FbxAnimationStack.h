//
//  FbxAnimationStack.h
//  FbxMesh
//
//  Created by jimCheng on 15/7/12.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__FbxAnimationStack__
#define __FbxMesh__FbxAnimationStack__

class FbxAnimationStack
{
public:
    FbxAnimationStack();
    ~FbxAnimationStack();
    
    bool setName(const char* name);
    const char* getName();
    
private:
    FbxAnimationStack(const FbxAnimationStack&);
    FbxAnimationStack& operator=(const FbxAnimationStack&);
    
    char* name;
    
    void releaseName();
};

#endif /* defined(__FbxMesh__FbxAnimationStack__) */
