//
//  CFBPlayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPlayer.h"
#include "CFBMatch.h"

void CFBPlayer::update(float dt)
{
    if (m_isBallController)
    {
        auto ball = FBMATCH->getBall();
        CC_ASSERT(ball->m_ownerPlayer == this);
        
        ball->setBallPos(m_curPosition);
    }
}

void CFBPlayer::gainBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = true;
    
    FBMATCH->getBall()->m_ownerPlayer = this;
    FBMATCH->getBall()->m_ownerTeam = m_ownerTeam;
}