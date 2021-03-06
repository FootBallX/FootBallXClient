//
//  CFBPlayerAI.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-19.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBPlayerAI__
#define __FootBallX__CFBPlayerAI__

#include "Common.h"
#include "FBDefs.h"

class CFBPlayer;
class CFBTeam;


class CFBPlayerAI
{
public:
    enum class AI_TYPE
    {
        GOALKEEPER,
        BACK,
        HALF_BACK,
        FORWARD,
    };
    
    CFBPlayerAI() = default;
    virtual ~CFBPlayerAI() = default;
    
    virtual bool init(CFBTeam* team, CFBPlayer* player, const cocos2d::Point& homePos, float orbit);
    
    virtual void setNetworkControl(bool networkControl);
    
    virtual void think();
    
    virtual void PreventOffside(float& x);
    
    virtual void update(float dt);
    
//    virtual void initPlayerStates(const cocos2d::Point& pt, bool networkControl);
    
    virtual CFBPlayer* getPlayer() const {return m_player;}

    virtual int getPassBallScore() const { return m_passBallScore; }
    
    virtual void setPassBallScore(int s) { m_passBallScore = s; }
    
    virtual void increasePassBallScore(int inc);
    
    virtual void thinkControlBall();        // AI控制，带球球员的AI
    virtual void updateAIControlBall(float dt);
protected:
    virtual void updateHomePosition() = 0;
    
    virtual void thinkOnAttacking();
    virtual void thinkOnDefending();
    
    virtual void considerChase();
    virtual void considerSupport();
    
    virtual void thinkDribbleBall();
    virtual void thinkPassBall();

    virtual void startWait(float t);
    virtual void updateWait(float dt);

    CFBTeam* m_team = nullptr;        // weak reference to the team object.
    CFBPlayer* m_player = nullptr;      // weak refrence to the owner player.
//    cocos2d::Point m_initPosition;
    cocos2d::Point m_origHomePosition;
    cocos2d::Point m_homePosition;
    float m_defendOrbitRadius;
    float m_defendOrbitRadiusSq;
    float m_defendOrbitRadiusx2Sq;
    
    FBDefs::AI_STATE m_state = FBDefs::AI_STATE::NONE;
    FBDefs::AI_STATE_CONTROL m_controlState = FBDefs::AI_STATE_CONTROL::NONE;
    
    cocos2d::Point m_moveToTarget;
    
    float m_waitTime = 0.f;
    
    int m_passBallScore = 0;
protected:      // ai logic functions
    virtual void returnToHome(float dt);
    virtual void chaseBall(float dt);
};



#endif /* defined(__FootBallX__CFBPlayerAI__) */
