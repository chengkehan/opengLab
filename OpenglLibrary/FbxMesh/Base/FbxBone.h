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
    
private:
    FbxBone(const FbxBone&);
    FbxBone operator=(const FbxBone&);
    
    char* name;
    BetterList<FbxBone*> childrenBones;
};

#endif /* defined(__FbxMesh__FbxBone__) */
