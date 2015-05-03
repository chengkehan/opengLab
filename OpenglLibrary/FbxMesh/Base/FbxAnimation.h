//
//  FbxAnimation.h
//  FbxMesh
//
//  Created by jimCheng on 15/5/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__FbxAnimation__
#define __FbxMesh__FbxAnimation__

#include "fbxsdk.h"

class FbxAnimation
{
public:
    FbxAnimation();
    ~FbxAnimation();
    
    bool loadFbxFromFile(const char* fbxFilePath);
    
private:
    FbxAnimation(const FbxAnimation&);
    const FbxAnimation operator=(const FbxAnimation&);
};

#endif /* defined(__FbxMesh__FbxAnimation__) */
