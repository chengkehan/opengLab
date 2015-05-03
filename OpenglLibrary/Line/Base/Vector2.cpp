//
//  Vector2.cpp
//  SimpleShape
//
//  Created by jimCheng on 15/3/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#include "Vector2.h"

/* PUBLIC */

Vector2::Vector2() :
    x(0.0f), y(0.0f)
{
    // Do nothing
}

Vector2::Vector2(float x, float y)
{
    this->x = x;
    this->y = y;
}

Vector2::Vector2(const Vector2& value)
{
    this->x = value.x;
    this->y = value.y;
}

Vector2::~Vector2()
{
    // Do nothing
}

//Vector2 Vector2::operator=(const Vector2 &value)
//{
//    return Vector2(value.x, value.y);
//}
