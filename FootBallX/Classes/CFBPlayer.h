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
#include "FBDefs.h"
#include "CFBPlayerInitInfo.h"

class CFBPlayerAI;

class CFBPlayer
{
public:
    CFBPlayer(CFBTeam* team, const CFBCard& card);
    
    virtual void update (float dt);
    virtual bool createBrain(FBDefs::AI_CLASS aiClass, const Point& homePos, float orbit);
    virtual CFBPlayerAI* getBrain();
    virtual CFBTeam* getOwnerTeam() const;
    
    virtual const CFBCard& getPlayerCard() const { return m_playerCard; }
    
    virtual void setPosition(const cocos2d::Point& pos);
    virtual const cocos2d::Point& getPosition() { return m_curPosition; }
    
    virtual void setMovingVector(const cocos2d::Point& vec);
    virtual void setMovingVector(float x, float y);
    virtual const cocos2d::Point& getMovingVector() { return m_movingVector; }
    virtual bool moveTo(const cocos2d::Point& pos, float dt = 0.f);
    virtual bool moveFromTo(const cocos2d::Point& pos, const cocos2d::Point& vec, float dt, float duration);
    
    virtual void setInstruction(FBDefs::PLAYER_INS ins) { m_instruction = ins; }
    virtual FBDefs::PLAYER_INS getInstruction() { return m_instruction; }
#pragma mark -- player actions
    virtual void gainBall();
    virtual void loseBall();
    virtual void stun();
    virtual bool isStunned();
#pragma mark -- Player states
    
    int m_positionInFormation = -1;
    float m_distanceFromBall = FLT_MAX;
    float m_radiusOfOrbit = 0.f;
    bool m_isBallController = false;
    bool m_isGoalKeeper = false;


protected:
    virtual float getSpeed();
    
    CFBPlayerAI* m_brain = nullptr;
    
    cocos2d::Point m_curPosition;
    cocos2d::Point m_movingVector;      // 运动方向
    cocos2d::Point m_targetPosition;
    
    FBDefs::PLAYER_INS m_instruction;
    
    float m_stunTime = 0.f;
#pragma mark -- player properties
    // all properties are measured by the pitch's width.
    CFBTeam* m_ownerTeam = nullptr;
    CFBCard m_playerCard;
    
    float m_speedCache = -FLT_MAX;
    float m_speedScale = 1.f;
};

#endif /* defined(__FootBallX__CFBPlayer__) */
