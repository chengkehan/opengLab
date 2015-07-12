//
//  FbxBone.h
//  FbxMesh
//
//  Created by jimCheng on 15/6/14.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__FbxBone__
#define __FbxMesh__FbxBone__

#include "BetterList.cpp"
#include "math3d.h"

class FbxBone
{
public:
    FbxBone();
    ~FbxBone();
    
    bool setName(const char* name);
    const char* getName();
    
    bool addChild(FbxBone* bone);
    FbxBone* getChild(unsigned int index);
    unsigned int numChildren();
    
    bool setIndices(const int* indices, unsigned int numIndices);
    const int* getIndices();
    
    bool setWeights(const double* weights, unsigned int numWeights);
    const double* getWeights();
    
    bool setNumIndices(unsigned int numIndices);
    unsigned int getNumIndices();
    
    bool setBindpose(const M3DMatrix44f bindpose);
    const M3DMatrix44f& getBindpose();
    
    bool setBindposeInverse(const M3DMatrix44f bindposeInverse);
    const M3DMatrix44f& getBindposeInverse();
    
private:
    FbxBone(const FbxBone&);
    FbxBone operator=(const FbxBone&);
    
    char* name;
    BetterList<FbxBone*> childrenBones;
    int* indices;
    double* weights;
    unsigned int numIndices;
    M3DMatrix44f bindpose;
    M3DMatrix44f bindposeInverse;
    
    void releaseIndices();
    void releaseWeights();
    void releaseName();
};

#endif /* defined(__FbxMesh__FbxBone__) */
