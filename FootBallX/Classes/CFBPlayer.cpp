//
//  CFBPlayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPlayer.h"


void CFBPlayer::gainBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = true;
}