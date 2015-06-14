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
    static unsigned int MAX_CHARS_OF_NAME;
    static unsigned int MAX_CHILDREN_BONES_AMOUNT;
    
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
    
    // MAX_CHARS_OF_NAME + 1/*end of char*/
    char name[21];
    
    // MAX_CHILDREN_BONES_AMOUNT
    FbxBone* childrenBones[20];
    unsigned int numChildrenBones;
    
    void emptyName();
};

#endif /* defined(__FbxMesh__FbxBone__) */
