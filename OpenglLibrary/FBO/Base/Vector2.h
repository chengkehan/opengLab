//
//  Vector2.h
//  SimpleShape
//
//  Created by jimCheng on 15/3/1.
//  Copyright (c) 2015年 JimCheng. All rights reserved.
//

#ifndef __SimpleShape__Vector2__
#define __SimpleShape__Vector2__

class Vector2
{
public:
    float x, y;
    
public:
    Vector2();
    Vector2(float x, float y);
    Vector2(const Vector2& value);
    ~Vector2();
    
//    Vector2 operator=(const Vector2& value);
};

#endif /* defined(__SimpleShape__Vector2__) */
