//
//  matrix_util.h
//  FbxMesh
//
//  Created by jimCheng on 15/7/11.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __FbxMesh__matrix_util__
#define __FbxMesh__matrix_util__

#include "fbxsdk.h"
#include "math3d.h"

void fbxAMatrix_to_m3dMatrix(FbxAMatrix* fbxAMatrix, M3DMatrix44f m3dMatrix);

#endif /* defined(__FbxMesh__matrix_util__) */
