//
//  CFBBall.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-27.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBBall.h"
#include "CFBPlayer.h"

void CFBBall::setBallPos(const Point& pos)
{
    CC_ASSERT(m_ownerPlayer);
    m_curPosition = pos;
    m_ownerPlayer->m_curPosition = pos;
}