//
//  matrix_util.cpp
//  FbxMesh
//
//  Created by jimCheng on 15/7/11.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "matrix_util.h"

void fbxAMatrix_to_m3dMatrix(FbxAMatrix* fbxAMatrix, M3DMatrix44f m3dMatrix)
{
    m3dMatrix[0] = (*fbxAMatrix)[0][0]; m3dMatrix[4] = (*fbxAMatrix)[1][0]; m3dMatrix[8] = (*fbxAMatrix)[2][0]; m3dMatrix[12] = (*fbxAMatrix)[3][0];
    m3dMatrix[1] = (*fbxAMatrix)[0][1]; m3dMatrix[5] = (*fbxAMatrix)[1][1]; m3dMatrix[9] = (*fbxAMatrix)[2][1]; m3dMatrix[13] = (*fbxAMatrix)[3][1];
    m3dMatrix[2] = (*fbxAMatrix)[0][2]; m3dMatrix[6] = (*fbxAMatrix)[1][2]; m3dMatrix[10] = (*fbxAMatrix)[2][2]; m3dMatrix[14] = (*fbxAMatrix)[3][2];
    m3dMatrix[3] = (*fbxAMatrix)[0][3]; m3dMatrix[7] = (*fbxAMatrix)[1][3]; m3dMatrix[11] = (*fbxAMatrix)[2][3]; m3dMatrix[15] = (*fbxAMatrix)[3][3];
}