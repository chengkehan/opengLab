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

FbxAnimation::FbxAnimation()
{
    
}

FbxAnimation::~FbxAnimation()
{
    
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

    FbxArray<FbxString*> animations;
    fbxScene->FillAnimStackNameArray(animations);
    printf("num animations:%d\n", animations.Size());
    for (int i = 0; i < animations.Size(); ++i)
    {
        printf("    %s\n", animations.GetAt(i)->Buffer());
    }
    
    printf("num pos:%d\n", fbxScene->GetPoseCount());
    for (int i = 0; i < fbxScene->GetPoseCount(); ++i)
    {
        printf("    pose name:%s\n", fbxScene->GetPose(i)->GetName());
    }

    printf("%s\n", fbxScene->GetPose(0) == nullptr ? "nopose" : "apose");
    
    DestroySdkObjects(fbxManager, fbxResult);
    return fbxResult;
}

/* PRIVATE */