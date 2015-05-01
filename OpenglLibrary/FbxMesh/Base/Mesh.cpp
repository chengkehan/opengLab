//
//  Mesh.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/4/5.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "Mesh.h"
#include "AttributeBindingLocation.h"

/* PUBLIC */

Mesh::Mesh() :
    vertices(nullptr), numOfVertices(0),
    normals(nullptr), numOfNormals(0),
    uv(nullptr), numOfUV(0),
    indices(nullptr), numOfIndices(0),
    compactAttributesVBO(0), compactIndicesVBO(0)
{
    // Do nothing
}

Mesh::~Mesh()
{
    releaseVertices();
    releaseNormals();
    releaseUV();
    releaseIndices();
    releaseCompactAttributesVBO();
}

bool Mesh::setVertices(const Vector3 *vertices, unsigned int count)
{
    releaseVertices();
    numOfVertices = count;
    return copyAttributes(reinterpret_cast<void**>(&this->vertices), vertices, sizeof(Vector3) * count);
}

bool Mesh::setNormals(const Vector3 *normals, unsigned int count)
{
    releaseNormals();
    numOfNormals = count;
    return copyAttributes(reinterpret_cast<void**>(&this->normals), normals, sizeof(Vector3) * count);
}

bool Mesh::setUV(const Vector2 *uv, unsigned count)
{
    releaseUV();
    numOfUV = count;
    return copyAttributes(reinterpret_cast<void**>(&this->uv), uv, sizeof(Vector2) * count);
}

bool Mesh::setIndices(const unsigned short *indices, unsigned int count)
{
    releaseIndices();
    numOfIndices = count;
    return copyAttributes(reinterpret_cast<void**>(&this->indices), indices, sizeof(unsigned short) * count);
}

bool Mesh::upload()
{
    if (vertices == nullptr || indices == nullptr || numOfVertices == 0 || numOfIndices == 0)
    {
        return false;
    }
    
    char* compactAttributes = static_cast<char*>(malloc(sizeof(Vector3) * numOfVertices + sizeof(Vector3) * numOfNormals + sizeof(Vector2) * numOfUV));
    if (compactAttributes == nullptr)
    {
        return false;
    }
    
    unsigned int ptrOffset = 0;
    memcpy(compactAttributes + ptrOffset, vertices, sizeof(Vector3) * numOfVertices);
    ptrOffset += sizeof(Vector3) * numOfVertices;
    if (numOfNormals > 0)
    {
        memcpy(compactAttributes + ptrOffset, normals, sizeof(Vector3) * numOfNormals);
        ptrOffset += sizeof(Vector3) * numOfNormals;
    }
    if (numOfUV > 0)
    {
        memcpy(compactAttributes + ptrOffset, uv, sizeof(Vector2) * numOfUV);
    }
    
    releaseCompactAttributesVBO();
    glGenBuffers(1, &compactAttributesVBO);
    glBindBuffer(GL_ARRAY_BUFFER, compactAttributesVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vector3) * numOfVertices + sizeof(Vector3) * numOfNormals + sizeof(Vector2) * numOfUV, compactAttributes, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    free(compactAttributes);
    
    glGenBuffers(1, &compactIndicesVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compactIndicesVBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned short) * numOfIndices, indices, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    
    releaseVertices();
    releaseNormals();
    releaseUV();
    releaseIndices();
    
    numOfVerticesForVBO = numOfVertices;
    numOfNormalsForVBO = numOfNormals;
    numOfIndicesForVBO = numOfIndices;
    numOfUvForVBO = numOfUV;
    
    return true;
}

bool Mesh::draw()
{
    if (compactAttributesVBO == 0 || compactIndicesVBO == 0)
    {
        return false;
    }
    
    unsigned int ptrOffset = 0;
    glBindBuffer(GL_ARRAY_BUFFER, compactAttributesVBO);
    glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_POSITION, 3/*xyz*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(ptrOffset));
    glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_POSITION);
    ptrOffset += sizeof(Vector3) * numOfVerticesForVBO;
    
    if(numOfNormalsForVBO > 0)
    {
        glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_NORMAL, 3/*xyz*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(ptrOffset));
        glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_NORMAL);
        ptrOffset += sizeof(Vector3) * numOfNormalsForVBO;
    }
    
    if (numOfUvForVBO > 0)
    {
        glVertexAttribPointer(ATTRIBUTE_BINDING_LOCATION_TEXTURE0, 2/*xy*/, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<const GLvoid*>(ptrOffset));
        glEnableVertexAttribArray(ATTRIBUTE_BINDING_LOCATION_TEXTURE0);
    }
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compactIndicesVBO);
    glDrawElements(GL_TRIANGLES, numOfIndicesForVBO, GL_UNSIGNED_SHORT, nullptr);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    return true;
}

/* PRIVATE */

void Mesh::releaseVertices()
{
    if (vertices != nullptr)
    {
        free(vertices);
        vertices = nullptr;
    }
}

void Mesh::releaseNormals()
{
    if (normals != nullptr)
    {
        free(normals);
        normals = nullptr;
    }
}

void Mesh::releaseIndices()
{
    if (indices != nullptr)
    {
        free(indices);
        indices = nullptr;
    }
}

void Mesh::releaseUV()
{
    if (uv != nullptr)
    {
        free(uv);
        uv = nullptr;
    }
}

void Mesh::releaseCompactAttributesVBO()
{
    glDeleteBuffers(1, &compactAttributesVBO);
    compactAttributesVBO = 0;
    glDeleteBuffers(1, &compactIndicesVBO);
    compactIndicesVBO = 0;
}

bool Mesh::copyAttributes(void **dest, const void *src, unsigned int numBytes)
{
    if (src == nullptr)
    {
        return false;
    }
    if (numBytes == 0)
    {
        return false;
    }
    
    *dest = malloc(numBytes);
    if (*dest == nullptr)
    {
        return false;
    }
    memcpy(*dest, src, numBytes);
    
    return true;
}


