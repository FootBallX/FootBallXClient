//
//  CFBPitch.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-10.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPitch.h"


Point CFBPitch::transformPersentage(const Point& pt)
{
    return Point(pt.x * m_width, pt.y * m_height);
}