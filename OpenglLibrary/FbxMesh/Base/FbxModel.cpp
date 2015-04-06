//
//  FbxModel.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxModel.h"
#include "Common.h"

/* PUBLIC */

FbxModel::FbxModel() :
    subMeshes(nullptr), numSubMeshes(0)
{
    // Do nothing
}

FbxModel::~FbxModel()
{
    release();
}

bool FbxModel::readFbxFromFile(const char *fbxFilePath)
{
    release();
    
    FbxManager* fbxManager = nullptr;
    FbxScene* fbxScene = nullptr;
    bool fbxResult;
    InitializeSdkObjects(fbxManager, fbxScene);
    
    fbxResult = LoadScene(fbxManager, fbxScene, fbxFilePath);
    if (!fbxResult)
    {
        FBXSDK_printf("Load fbx fail:%s\n", fbxFilePath);
    }
    else
    {
//        FBXSDK_printf("Load fbx success.\n");
        
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

void FbxModel::processFbxNode(FbxNode *fbxNode)
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
            case FbxNodeAttribute::eMesh :
            {
                processFbxMesh(fbxNode);
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

void FbxModel::processFbxMesh(FbxNode *fbxNode)
{
    FbxMesh* fbxMesh = dynamic_cast<FbxMesh*>(fbxNode->GetNodeAttribute());
//    FBXSDK_printf("Mesh name:%s\n", fbxNode->GetName());
    processVertexPosition(fbxMesh);
    processPolygon(fbxMesh);
}

void FbxModel::processVertexPosition(FbxMesh* fbxMesh)
{

}

void FbxModel::processPolygon(FbxMesh* fbxMesh)
{
    
}

void FbxModel::release()
{
}
