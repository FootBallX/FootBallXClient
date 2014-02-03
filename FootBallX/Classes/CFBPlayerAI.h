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
class CFBFormation;

#pragma mark ----- AI Base

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
    
    virtual bool init(CFBFormation* formation, CFBPlayer* player, float posX, float posY, float radius);
    
    virtual void think();
    
    virtual void update(float dt);
    
    virtual void initPlayerStates();
    
    virtual CFBPlayer* getPlayer() const {return m_player;}
    
    virtual void thinkHomePosition();
    
    virtual void thinkDefending();
protected:
    virtual void updatePlayerStates();
    virtual bool isOnHomePosition();
    virtual void applyStateCD() { m_changeStateCD = 1.f; }
    virtual bool isNotInStateCD() { return true; return FLT_LE(m_changeStateCD, 0.f); }

    CFBFormation* m_formation = nullptr;        // weak reference to the formation object.
    CFBPlayer* m_player = nullptr;
    Point m_attackPosition;               // percentage of the pitch
    Point m_defendPosition;
    Point m_homePosition;
    float m_defendOrbitRadius;
    float m_defendOrbitRadiusSq;
    float m_defendOrbitRadiusx2Sq;
    
    FBDefs::AI_STATE m_state = FBDefs::AI_STATE::NONE;
    float m_changeStateCD = 0.f;
protected:      // ai logic functions
    virtual void returnToPosition(float dt);
    virtual void chaseBall(float dt);
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
