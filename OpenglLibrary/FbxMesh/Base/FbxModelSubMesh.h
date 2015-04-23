//
//  FbxModelSubMesh.h
//  FbxMesh
//
//  Created by jimCheng on 15/4/6.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__FbxModelSubMesh__
#define __FbxMesh__FbxModelSubMesh__

#include "Vector3.h"
#include "Vector2.h"
#include "BetterList.cpp"

class FbxModelSubMesh
{
public:
    BetterList<Vector3>* vertices;
    
    BetterList<Vector2>* uv;
    
    BetterList<Vector3>* normals;
    
    BetterList<unsigned short>* indices;
    
public:
    FbxModelSubMesh();
    ~FbxModelSubMesh();
    
private:
    FbxModelSubMesh(const FbxModelSubMesh&);
    FbxModelSubMesh operator=(const FbxModelSubMesh&);
    
    void release();
};

#endif /* defined(__FbxMesh__FbxModelSubMesh__) */
