//
//  Vector4.h
//  SimpleShape
//
//  Created by jimCheng on 15/3/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __SimpleShape__Vector4__
#define __SimpleShape__Vector4__

class Vector4
{
public:
    float x, y, z, w;
    
public:
    Vector4();
    Vector4(float x, float y, float z, float w);
    Vector4(const Vector4& value);
    ~Vector4();
    
//    Vector4 operator=(const Vector4& value);
};

#endif /* defined(__SimpleShape__Vector4__) */
