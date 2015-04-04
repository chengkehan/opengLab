//
//  Vector3.cpp
//  SimpleShape
//
//  Created by jimCheng on 15/3/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "Vector3.h"

/* PUBLIC */

Vector3::Vector3() :
    x(0.0f), y(0.0f), z(0.0f)
{
    // Do nothing
}

Vector3::Vector3(float x, float y, float z)
{
    this->x = x;
    this->y = y;
    this->z = z;
}

Vector3::Vector3(const Vector3& value)
{
    this->x = value.x;
    this->y = value.y;
    this->z = value.z;
}

Vector3::~Vector3()
{
    // Do nothing
}

//Vector3 Vector3::operator=(const Vector3 &value)
//{
//    return Vector3(value.x, value.y, value.z);
//}
