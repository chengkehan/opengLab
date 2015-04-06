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

class FbxModel
{
public:
    FbxModel();
    ~FbxModel();
    
    bool readFbxFromFile(const char* fbxFilePath);
    
    const Vector3* getVertices();
    unsigned int getNumVertices();
    
    const Vector2* getUV();
    unsigned int getNumUV();
    
    const Vector3* getNormals();
    unsigned int getNumNormals();
    
private:
    FbxModelSubMesh* subMeshes;
    unsigned int numSubMeshes;
    
private:
    FbxModel(const FbxModel&);
    const FbxModel operator=(const FbxModel&);
    
    void processFbxNode(FbxNode* fbxNode);
    void processFbxMesh(FbxNode* fbxNode);
    void processVertexPosition(FbxMesh* fbxMesh);
    void processPolygon(FbxMesh* fbxMesh);
    
    void release();
};

#endif /* defined(__FbxMesh__FbxModel__) */
