//
//  Vector4.cpp
//  SimpleShape
//
//  Created by jimCheng on 15/3/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "Vector4.h"

/* PUBLIC */

Vector4::Vector4() :
    x(0.0f), y(0.0f), z(0.0f), w(0.0f)
{
    // Do nothing
}

Vector4::Vector4(float x, float y, float z, float w)
{
    this->x = x;
    this->y = y;
    this->z = z;
    this->w = w;
}

Vector4::Vector4(const Vector4& value)
{
    this->x = value.x;
    this->y = value.y;
    this->z = value.z;
    this->w = value.w;
}

Vector4::~Vector4()
{
    // Do nothing
}

//Vector4 Vector4::operator=(const Vector4 &value)
//{
//    return Vector4(value.x, value.y, value.z, value.w);
//}
