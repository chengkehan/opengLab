//
//  Mesh.h
//  SimpleShape
//
//  Created by jimCheng on 15/2/24.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __SimpleShape__Mesh__
#define __SimpleShape__Mesh__

#include "Vector3.h"
#include "Vector2.h"
#include "Vector4.h"

class Mesh
{
public:
    Mesh();
    ~Mesh();
    
    bool setVertices(Vector3 vertices[], unsigned int numVertices);
    bool getVertices(Vector3 vertices[], unsigned int* numVertices);
    
    bool setNormals(Vector3 normals[], unsigned int numNormals);
    bool getNormals(Vector3 normals[], unsigned int* numNormals);
    
    bool setUVs(Vector2 uvs[], unsigned int numUVs);
    bool getUVs(Vector2 uvs[], unsigned int* numUVs);
    
    bool setColors(Vector4 colors[], unsigned int numColors);
    bool getColors(Vector4 colors[], unsigned int* numColors);
    
    bool setTangents(Vector3 tangents[], unsigned int numTangents);
    bool getTangents(Vector3 tangents[], unsigned int* numTangents);
    
private:
    Mesh(const Mesh&);
    const Mesh operator=(const Mesh&);
};

#endif /* defined(__SimpleShape__Mesh__) */
