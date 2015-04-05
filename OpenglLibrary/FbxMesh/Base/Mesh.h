//
//  Mesh.h
//  FbxMesh
//
//  Created by jimCheng on 15/4/5.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__Mesh__
#define __FbxMesh__Mesh__

#include "Vector3.h"
#include "Vector2.h"
#include "glHeader.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    bool setVertices(const Vector3* vertices, unsigned int count);
    bool setUV(const Vector2* uv, unsigned int count);
    bool setIndices(const unsigned short* indices, unsigned int count);
    
    bool upload();
    bool draw();
    
private:
    Mesh(const Mesh&);
    const Mesh operator=(const Mesh&);
    
    Vector3* vertices;
    unsigned int numOfVertices;
    unsigned int numOfVerticesForVBO;
    
    Vector2* uv;
    unsigned int numOfUV;
    unsigned int numOfUvForVBO;
    
    unsigned short* indices;
    unsigned int numOfIndices;
    unsigned int numOfIndicesForVBO;
    
    GLuint compactAttributesVBO;
    GLuint compactIndicesVBO;
    
private:
    void releaseVertices();
    void releaseUV();
    void releaseIndices();
    void releaseCompactAttributesVBO();
    
    bool copyAttributes(void** dest, const void* src, unsigned int numBytes);
};

#endif /* defined(__FbxMesh__Mesh__) */
