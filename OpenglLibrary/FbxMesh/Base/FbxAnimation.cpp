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
    else
    {
        printf("num pose:%d\n", fbxScene->GetPoseCount());
        
        FbxGeometryConverter fbxGeometryConverter(fbxManager);
        fbxGeometryConverter.Triangulate(fbxScene, true);
        
        FbxNode* fbxNode = fbxScene->GetRootNode();
        if (fbxNode != nullptr)
        {
            for (int i = 0; i < fbxNode->GetChildCount(); ++i)
            {
                processFbxNode(fbxNode->GetChild(i));
            }
        }

    }
    
    DestroySdkObjects(fbxManager, fbxResult);
    return fbxResult;
}

/* PRIVATE */

void FbxAnimation::processFbxNode(FbxNode *fbxNode)
{
    if (fbxNode == nullptr)
    {
        return;
    }
    
    if (fbxNode->GetNodeAttribute() == nullptr)
    {
        FBXSDK_printf("Null node attribute\n");
    }
    else
    {
        FbxNodeAttribute::EType fbxNodeAttributeType = fbxNode->GetNodeAttribute()->GetAttributeType();
        switch (fbxNodeAttributeType)
        {
            case FbxNodeAttribute::eSkeleton :
            {
                printf("skeleton: %s\n", fbxNode->GetName());
                FbxAMatrix m = fbxNode->EvaluateGlobalTransform();
                FbxVector4 t = m.GetT();
                printf("    t: %lf, %lf, %lf, %lf\n", t.mData[0], t.mData[1], t.mData[2], t.mData[3]);
                FbxVector4 r = m.GetR();
                printf("    r: %lf, %lf, %lf, %lf\n", r.mData[0], r.mData[1], r.mData[2], r.mData[3]);
                FbxVector4 s = m.GetS();
                printf("    s: %lf, %lf, %lf, %lf\n", s.mData[0], s.mData[1], s.mData[2], s.mData[3]);
                break;
            }
            default:
                break;
        }
    }
    
    for (int i = 0; i < fbxNode->GetChildCount(); ++i)
    {
        processFbxNode(fbxNode->GetChild(i));
    }
}