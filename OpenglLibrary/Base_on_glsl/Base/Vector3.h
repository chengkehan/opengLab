//
//  Vector3.h
//  SimpleShape
//
//  Created by jimCheng on 15/3/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __SimpleShape__Vector3__
#define __SimpleShape__Vector3__

class Vector3
{
public:
    float x, y, z;
    
public:
    Vector3();
    Vector3(float x, float y, float z);
    Vector3(const Vector3& value);
    ~Vector3();
    
//    Vector3 operator=(const Vector3& value);
};

#endif /* defined(__SimpleShape__Vector3__) */
