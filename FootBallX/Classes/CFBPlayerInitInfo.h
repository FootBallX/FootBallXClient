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
#include "CFBCard.h"

class CFBPlayerInitInfo
{
public:    
    CFBCard card;
    Point position;
    Point homePosition;
    int aiClass;
};

#endif /* defined(__FootBallX__CFBPlayerInitInfo__) */