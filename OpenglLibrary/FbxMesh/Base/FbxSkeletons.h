//
//  FbxSkeleton.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__FbxSkeleton__
#define __FbxMesh__FbxSkeleton__

#include "FbxBone.h"
#include "fbxsdk.h"

class FbxSkeletons
{
public:
    static unsigned int MAX_BONES_AMOUNT;
    
    FbxSkeletons();
    ~FbxSkeletons();
    
    bool parseFromFile(const char* fbxFilePath);
    void printTreeStruct();
    
private:
    FbxSkeletons(const FbxSkeletons&);
    FbxSkeletons operator=(const FbxSkeletons&);
    
    // MAX_BONES_AMOUNT
    FbxBone bones[150];
    unsigned int numBones;
    
    void processSkeleton(FbxNode *fbxNode, int parentIndex);
    void processMesh(FbxNode* fbxNode);
    void processGeometry(FbxGeometry* fbxGeometry);
    void printTreeStructRecursively(FbxBone* bone, unsigned int indent);
    FbxBone* getBone(const char* boneName);
};

#endif /* defined(__FbxMesh__FbxSkeleton__) */
