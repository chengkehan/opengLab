//
//  FbxSkeleton.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/6/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxSkeletons.h"
#include "Common.h"

unsigned int FbxSkeletons::MAX_BONES_AMOUNT = 150;

/* PUBLIC */

FbxSkeletons::FbxSkeletons() :
    numBones(0)
{
    
}

FbxSkeletons::~FbxSkeletons()
{
    
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
            for (int i = 0; i < fbxNode->GetChildCount(); ++i)
            {
                processFbxNode(fbxNode->GetChild(i), -1);
            }
        }
        
    }
    
    DestroySdkObjects(fbxManager, fbxResult);
    return fbxResult;
}

void FbxSkeletons::printTreeStruct()
{
    printf("FbxSkeletons Tree Struct:\n");
    if (numBones == 0)
    {
        printf("There is nothing");
    }
    else
    {
        printTreeStructRecursively(&bones[0], 0);
    }
}

/* PRIVATE */

void FbxSkeletons::processFbxNode(FbxNode *fbxNode, int parentIndex)
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
                if(numBones < FbxSkeletons::MAX_BONES_AMOUNT)
                {
                    if(parentIndex != -1 || numBones == 0)
                    {
                        FbxBone* bone = &bones[numBones];
                        bone->setName(fbxNode->GetName());
                        if (parentIndex != -1)
                        {
                            bones[parentIndex].addChild(bone);
                        }
                        ++numBones;
                        printf("%s\n", fbxNode->GetName());
                    }
                    else
                    {
                        printf("The root bone must be only one.\n");
                    }
                }
                else
                {
                    printf("Limited max amount of bones");
                }
                break;
            }
            default:
                break;
        }
    }
    
    unsigned int numBonesCurrent = numBones;
    for (int i = 0; i < fbxNode->GetChildCount(); ++i)
    {
        processFbxNode(fbxNode->GetChild(i), numBonesCurrent - 1);
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
