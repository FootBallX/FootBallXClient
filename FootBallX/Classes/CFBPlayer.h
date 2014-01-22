//
//  CFBPlayer.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBPlayer__
#define __FootBallX__CFBPlayer__

#include "Common.h"
#include "CFBTeam.h"

class CFBPlayer
{
public:
    CFBTeam::TEAM m_team = CFBTeam::TEAM::NUM;
    Point m_curPosition;
    
#pragma mark -- Player states
    float m_distanceFromBall = FLT_MAX;
    float m_radiusOfOrbit = 0.f;
    bool m_isBallController = false;
    bool m_isGoalKeeper = false;
    
    enum STATE
    {
        IDLE,
    };
    
    STATE m_state = STATE::IDLE;
protected:
private:

};

#endif /* defined(__FootBallX__CFBPlayer__) */
