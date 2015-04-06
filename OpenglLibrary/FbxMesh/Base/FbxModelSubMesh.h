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

struct FbxModelSubMesh
{
public:
    Vector3* vertices;
    unsigned int numVertices;
    
    Vector2* uv;
    unsigned int numUV;
    
    Vector3* normals;
    unsigned int numNormals;
    
    unsigned short* indices;
    unsigned int numIndices;
};

#endif /* defined(__FbxMesh__FbxModelSubMesh__) */
