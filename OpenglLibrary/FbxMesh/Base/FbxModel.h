//
//  FbxModel.h
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__FbxModel__
#define __FbxMesh__FbxModel__

#include "fbxsdk.h"
#include "FbxModelSubMesh.h"
#include "BetterList.cpp"

class FbxModel
{
public:
    FbxModel();
    ~FbxModel();
    
    bool readFbxFromFile(const char* fbxFilePath);
    unsigned int numSubMeshes();
    FbxModelSubMesh* getSubMesh(unsigned int index);
    
private:
    BetterList<FbxModelSubMesh*> subMeshes;
    
private:
    FbxModel(const FbxModel&);
    const FbxModel operator=(const FbxModel&);
    
    void processFbxNode(FbxNode* fbxNode);
    void processFbxMesh(FbxNode* fbxNode);
    
    void release();
    void releaseSubMeshes();
};

#endif /* defined(__FbxMesh__FbxModel__) */
