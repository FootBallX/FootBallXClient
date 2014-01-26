//
//  CFBPitch.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-10.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPitch.h"


Point CFBPitch::transformPersentage(const Point& pt, FBDefs::SIDE side)
{
    Point point(pt);
    if (side == FBDefs::SIDE::RIGHT)
    {
        point.x = 1 - point.x;
        point.y = 1 - point.y;
    }
    return Point(point.x * m_width, point.y * m_height);
}



float CFBPitch::transformPersentage(float p)
{
    return p * m_width;
}



void CFBPitch::setPitchSize(int w, int h)
{
    m_width = w;
    m_height = h;
}



