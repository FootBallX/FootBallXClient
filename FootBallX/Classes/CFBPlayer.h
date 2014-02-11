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
    CFBPlayer();
    
    virtual void update (float dt);
    
    virtual float getSpeed() const;
    virtual void setSpeed(float speed);
#pragma mark -- player actions
    virtual void gainBall();
    virtual void loseBall();
#pragma mark -- Player states
    cocos2d::Point m_curPosition;
    CFBTeam* m_ownerTeam = nullptr;
    int m_positionInFormation = -1;
    bool m_isOnDuty = false;        // 上场
    float m_distanceFromBall = FLT_MAX;
    float m_radiusOfOrbit = 0.f;
    bool m_isBallController = false;
    bool m_isGoalKeeper = false;

protected:
#pragma mark -- player properties
    // all properties are measured by the pitch's width.
    float m_speed = 0.f;
};

#endif /* defined(__FootBallX__CFBPlayer__) */
