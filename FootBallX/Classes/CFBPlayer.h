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
#include "CFBCard.h"

class CFBPlayer
{
public:
    CFBPlayer(const string& cid);
    CFBPlayer(const CFBCard& card);
    
    virtual void update (float dt);
    
    virtual float getSpeed();
    
    virtual const CFBCard& getPlayerCard() const { return m_playerCard; }
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
    const CFBCard& m_playerCard;
    
    float m_speedCache = -FLT_MAX;
};

#endif /* defined(__FootBallX__CFBPlayer__) */
