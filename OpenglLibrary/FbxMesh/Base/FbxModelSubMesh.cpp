//
//  FbxModelSubMesh.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/23.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "FbxModelSubMesh.h"

/* PUBLIC */

FbxModelSubMesh::FbxModelSubMesh()
{
    release();
}

FbxModelSubMesh::~FbxModelSubMesh()
{
    release();
}

/* PRIVATE */

void FbxModelSubMesh::release()
{
    vertices = nullptr;
    normals = nullptr;
    uv = nullptr;
    indices = nullptr;
}
