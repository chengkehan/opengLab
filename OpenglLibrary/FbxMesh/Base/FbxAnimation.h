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
#include "FbxSkeletons.h"
#include "BetterList.cpp"
#include "FbxAnimationStack.h"

class FbxAnimation
{
public:
    FbxAnimation(FbxSkeletons* fbxSkeletons);
    ~FbxAnimation();
    
    bool loadFbxFromFile(const char* fbxFilePath);
    
private:
    FbxAnimation(const FbxAnimation&);
    FbxAnimation operator=(const FbxAnimation&);
    
    FbxSkeletons* fbxSkeletons;
    BetterList<FbxAnimationStack*> animationStacks;
    
    void processAnimationStacks(FbxScene* fbxScene);
    void processAnimationLayers(FbxAnimStack* fbxAnimStack, FbxScene* fbxScene);
    void releaseClips();
};

#endif /* defined(__FbxMesh__FbxAnimation__) */
