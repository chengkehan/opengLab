//
//  FbxAnimation.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/5/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxAnimation.h"
#include "Common.h"

/* PUBLIC */

FbxAnimation::FbxAnimation(FbxSkeletons* fbxSkeletons) :
    fbxSkeletons(fbxSkeletons)
{
    // Do nothing
}

FbxAnimation::~FbxAnimation()
{
    releaseClips();
}

bool FbxAnimation::loadFbxFromFile(const char *fbxFilePath)
{
    FbxManager* fbxManager = nullptr;
    FbxScene* fbxScene = nullptr;
    bool fbxResult;
    InitializeSdkObjects(fbxManager, fbxScene);
    
    fbxResult = LoadScene(fbxManager, fbxScene, fbxFilePath);
    if (!fbxResult)
    {
        FBXSDK_printf("Load fbx fail:%s\n", fbxFilePath);
        return false;
    }
    else
    {
        FbxGeometryConverter fbxGeometryConverter(fbxManager);
        fbxGeometryConverter.Triangulate(fbxScene, true);
        
        processAnimationStacks(fbxScene);
    }
    
    DestroySdkObjects(fbxManager, fbxResult);
    return fbxResult;
}

/* PRIVATE */

void FbxAnimation::processAnimationStacks(FbxScene *fbxScene)
{
    for (int i = 0; i < fbxScene->GetSrcObjectCount<FbxAnimStack>(); ++i)
    {
        FbxAnimStack* fbxAnimStack = fbxScene->GetSrcObject<FbxAnimStack>(i);

        FbxAnimationStack* animationStack = Memory_NewHeapObject(FbxAnimationStack);
        animationStacks.add(animationStack);
        animationStack->setName(fbxAnimStack->GetName());
        printf("------------------------------------------%s\n", animationStack->getName());
        processAnimationLayers(fbxAnimStack, fbxScene);
    }
}

void FbxAnimation::processAnimationLayers(FbxAnimStack *fbxAnimStack, FbxScene *fbxScene)
{
    int numLayers = fbxAnimStack->GetMemberCount<FbxAnimLayer>();
    for (int i = 0; i < numLayers; ++i)
    {
        FbxAnimLayer* fbxAnimLayer = fbxAnimStack->GetMember<FbxAnimLayer>(i);
    }
}

void FbxAnimation::releaseClips()
{
    unsigned int numAnimationStacks = animationStacks.length();
    for (int i = 0; i < numAnimationStacks; ++i)
    {
        Memory_DeleteHeapObject(animationStacks[i]);
    }
    animationStacks.release();
}

