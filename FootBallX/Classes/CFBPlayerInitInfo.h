//
//  CFBPlayerInitInfo.h
//  FootBallX
//
//  Created by 马 俊 on 14-5-18.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBPlayerInitInfo__
#define __FootBallX__CFBPlayerInitInfo__

#include "Common.h"

class CFBPlayerInitInfo
{
public:
    CFBPlayerInitInfo(float x, float y, float x1, float y1, int a) :
    position(x, y),
    homePosition(x1, y1),
    aiClass(a)
    {}
    
    Point position;
    Point homePosition;
    int aiClass;
};

#endif /* defined(__FootBallX__CFBPlayerInitInfo__) */
