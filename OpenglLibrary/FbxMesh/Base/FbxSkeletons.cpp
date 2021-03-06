//
//  FbxSkeleton.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxSkeletons.h"
#include "Common.h"
#include "Memory.h"
#include "matrix_util.h"

/* PUBLIC */

FbxSkeletons::FbxSkeletons()
{
    
}

FbxSkeletons::~FbxSkeletons()
{
    releaseBones();
}

bool FbxSkeletons::parseFromFile(const char *fbxFilePath)
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
        
        FbxNode* fbxNode = fbxScene->GetRootNode();
        if (fbxNode != nullptr)
        {
            int numChildren = fbxNode->GetChildCount();
            
            for (int i = 0; i < numChildren; ++i)
            {
                processSkeleton(fbxNode->GetChild(i), -1);
            }
            
            for (int i = 0; i < numChildren; ++i)
            {
                processMesh(fbxNode->GetChild(i));
            }

        }
    }
    
    DestroySdkObjects(fbxManager, fbxResult);
    return fbxResult;
}

FbxBone* FbxSkeletons::getRootBone()
{
    return bones.length() == 0 ? nullptr : bones[0];
}

void FbxSkeletons::printTreeStruct()
{
    printf("FbxSkeletons Tree Struct:\n");
    if (bones.length() == 0)
    {
        printf("There is nothing");
    }
    else
    {
        printTreeStructRecursively(bones[0], 0);
    }
}

/* PRIVATE */

void FbxSkeletons::processSkeleton(FbxNode *fbxNode, int parentIndex)
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
                if(parentIndex != -1 || bones.length() == 0)
                {
                    FbxBone* newBone = Memory_NewHeapObject(FbxBone);
                    bones.add(newBone);
                    newBone->setName(fbxNode->GetName());
                    if (parentIndex != -1)
                    {
                        bones[parentIndex]->addChild(newBone);
                    }
                }
                else
                {
                    printf("The root bone must be only one.\n");
                }
                break;
            }
            default:
                break;
        }
    }
    
    unsigned int numBonesCurrent = bones.length();
    int numChildren = fbxNode->GetChildCount();
    for (int i = 0; i < numChildren; ++i)
    {
        processSkeleton(fbxNode->GetChild(i), numBonesCurrent - 1);
    }
}

void FbxSkeletons::processMesh(FbxNode *fbxNode)
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
                FbxMesh* mesh = (FbxMesh*)fbxNode->GetNodeAttribute();
                processGeometry(mesh);
                break;
            }
            default:
                break;
        }
    }
    
    int numChildren = fbxNode->GetChildCount();
    for (int i = 0; i < numChildren; ++i)
    {
        processMesh(fbxNode->GetChild(i));
    }

}

void FbxSkeletons::processGeometry(FbxGeometry *fbxGeometry)
{
    int skinCount = fbxGeometry->GetDeformerCount(FbxDeformer::eSkin);
    for (int skinIndex = 0; skinIndex < skinCount; ++skinIndex)
    {
        FbxSkin* fbxSkin = (FbxSkin*)fbxGeometry->GetDeformer(skinIndex, FbxDeformer::eSkin);
        int clusterCount = fbxSkin->GetClusterCount();
        for (int clusterIndex = 0; clusterIndex < clusterCount; ++clusterIndex)
        {
            FbxCluster* cluster = fbxSkin->GetCluster(clusterIndex);
            const char* boneName = cluster->GetLink()->GetName();
            FbxBone* bone = getBone(boneName);
            if(bone == nullptr)
            {
                printf("The skin link to a bone, but cannot find the definition of the bone:%s\n", boneName);
            }
            else
            {
                int numControlPoints = cluster->GetControlPointIndicesCount();
                int* controlPointIndices = cluster->GetControlPointIndices();
                double* controlPointWeights = cluster->GetControlPointWeights();
                bone->setNumIndices(numControlPoints);
                bone->setIndices(controlPointIndices, numControlPoints);
                bone->setWeights(controlPointWeights, numControlPoints);
                
                // Bindpose
                {
                    FbxAMatrix matrix;
                    cluster->GetTransformLinkMatrix(matrix);
                    M3DMatrix44f bindpose;
                    fbxAMatrix_to_m3dMatrix(&matrix, bindpose);
                    bone->setBindpose(bindpose);
                }
                
                // Bindpose Inverse
                {
                    FbxAMatrix matrix;
                    cluster->GetTransformLinkMatrix(matrix);
                    FbxAMatrix inverse = matrix.Inverse();
                    M3DMatrix44f bindposeInverse;
                    fbxAMatrix_to_m3dMatrix(&inverse, bindposeInverse);
                    bone->setBindposeInverse(bindposeInverse);
                }
            }
        }
    }
}

void FbxSkeletons::printTreeStructRecursively(FbxBone* bone, unsigned int indent)
{
    if (bone == nullptr)
    {
        return;
    }
    
    for (int i = 0; i < indent; ++i)
    {
        printf("    ");
    }
    printf("%s\n", bone->getName());
    
    int numChildren = bone->numChildren();
    for(int i = 0; i < numChildren; ++i)
    {
        printTreeStructRecursively(bone->getChild(i), indent + 1);
    }
}

FbxBone* FbxSkeletons::getBone(const char *boneName)
{
    unsigned int numBones = bones.length();
    for (int i = 0; i < numBones; ++i)
    {
        FbxBone* bone = bones[i];
        if (bone->getName() == nullptr && boneName == nullptr)
        {
            return bone;
        }
        else if(bone->getName() == nullptr && boneName != nullptr)
        {
            return nullptr;
        }
        else if(bone->getName() != nullptr && boneName == nullptr)
        {
            return nullptr;
        }
        else
        {
            if(strcmp(bone->getName(), boneName) == 0)
            {
                return bone;
            }
        }
    }
    return nullptr;
}

void FbxSkeletons::releaseBones()
{
    unsigned int numBones = bones.length();
    for (int i = 0; i < numBones; ++i)
    {
        Memory_DeleteHeapObject(bones[i]);
    }
}
