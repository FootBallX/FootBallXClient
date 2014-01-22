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

class CFBPlayer;

#pragma mark ----- AI Base

class CFBPlayerAI
{
public:
    enum AI_TYPE
    {
        GOALKEEPER,
        BACK,
        HALF_BACK,
        FORWARD,
    };
    
    CFBPlayerAI() = default;
    virtual ~CFBPlayerAI() = default;
    
    virtual bool init(CFBPlayer* player, float posX, float posY, float radius);
    
    virtual void update(float dt) = 0;
    
    virtual void initPlayerStates();
protected:
    virtual void updatePlayerStates();

    CFBPlayer* m_player = nullptr;
    Point m_position;               // percentage of the pitch
    float m_radiusOfOrbitRate;
};

#pragma mark ----- GoalKeeper AI

class CFBGoalkeeperAI
: public CFBPlayerAI
{
public:
    CFBGoalkeeperAI() = default;
    virtual ~CFBGoalkeeperAI() = default;
    
    virtual void update(float dt) override;
    
protected:
    virtual void updatePlayerStates() override;
    virtual void initPlayerStates() override;
};

#pragma mark ----- Back AI

class CFBBackAI
: public CFBPlayerAI
{
public:
    CFBBackAI() = default;
    virtual ~CFBBackAI() = default;
    
    virtual void update(float dt) override;
    
protected:
    virtual void updatePlayerStates() override;
    virtual void initPlayerStates() override;
};

#pragma mark ----- Half back AI

class CFBHalfBackAI
: public CFBPlayerAI
{
public:
    CFBHalfBackAI() = default;
    virtual ~CFBHalfBackAI() = default;
    
    virtual void update(float dt) override;
    
    virtual void initPlayerStates() override;
    
protected:
    virtual void updatePlayerStates() override;
    
};

#pragma mark ----- Forward AI

class CFBForwardAI
: public CFBPlayerAI
{
public:
    CFBForwardAI() = default;
    virtual ~CFBForwardAI() = default;
    
    virtual void update(float dt) override;
    
protected:
    virtual void updatePlayerStates() override;
    virtual void initPlayerStates() override;
};

#endif /* defined(__FootBallX__CFBPlayerAI__) */
