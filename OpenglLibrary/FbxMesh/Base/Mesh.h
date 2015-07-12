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
#include "MeshRenderType.h"

class Mesh
{
public:
    Mesh(enum MESH_RENDER_TYPE meshRenderType);
    ~Mesh();
    
    bool setVertices(const Vector3* vertices, unsigned int count);
    bool setNormals(const Vector3* normals, unsigned int count);
    bool setUV(const Vector2* uv, unsigned int count);
    bool setColors(const Vector3* colors, unsigned int count);
    bool setIndices(const unsigned short* indices, unsigned int count);
    
    void setLineWidth(GLfloat lineWidth);
    
    bool upload();
    bool draw();
    
private:
    Vector3* vertices;
    unsigned int numOfVertices;
    unsigned int numOfVerticesForVBO;
    
    Vector3* normals;
    unsigned int numOfNormals;
    unsigned int numOfNormalsForVBO;
    
    Vector2* uv;
    unsigned int numOfUV;
    unsigned int numOfUvForVBO;
    
    Vector3* colors;
    unsigned int numOfColors;
    unsigned int numOfColorsForVBO;
    
    unsigned short* indices;
    unsigned int numOfIndices;
    unsigned int numOfIndicesForVBO;
    
    GLuint compactAttributesVBO;
    GLuint compactIndicesVBO;
    
    enum MESH_RENDER_TYPE meshRenderType;
    
    GLfloat lineWidth;
    
private:
    Mesh(const Mesh&);
    const Mesh operator=(const Mesh&);
    
    void releaseVertices();
    void releaseNormals();
    void releaseUV();
    void releaseIndices();
    void releaseColors();
    void releaseCompactAttributesVBO();
    
    bool copyAttributes(void** dest, const void* src, unsigned long numBytes);
};

#endif /* defined(__FbxMesh__Mesh__) */
