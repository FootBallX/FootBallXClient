//
//  CFBFormation.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBFormation__
#define __FootBallX__CFBFormation__

#include "Common.h"
#include "CFBPlayerAI.h"

class CFBPlayer;

#define GOALKEEPER_ORBIT_RATE                0.1f        // 10% pitch width
#define BACK_ORBIT_RATE                      0.15f       // 15%
#define HALF_BACK_ORBIT_RATE                 0.25f
#define FORWARD_ORBIT_RATE                   0.2f


#pragma mark ----- FormationBase

class CFBFormation
{
public:
    CFBFormation() = default;
    virtual ~CFBFormation();
    virtual const char* description() = 0;

    virtual bool init();
    virtual void addPlayer(CFBPlayer* player, int pos) = 0;
    
    virtual void update(float dt);
    
    virtual int getPlayerNumber() { return m_playerNumber;}
    
    virtual bool onStartMatch();
protected:
    int m_playerNumber = 0;
    CFBPlayerAI** m_playerAIs = nullptr;
private:
};

#pragma mark ----- 442

class CFBFormation442
: public CFBFormation
{
public:
    CFBFormation442();
    virtual ~CFBFormation442() = default;
    virtual bool init() override;
    virtual const char* description() override;
    virtual void addPlayer(CFBPlayer* player, int pos) override;
protected:
private:
};

#pragma mark ----- 352

class CFBFormation352
: public CFBFormation
{
public:
    CFBFormation352();
    virtual ~CFBFormation352() = default;
    virtual bool init() override;
    virtual const char* description() override;
    virtual void addPlayer(CFBPlayer* player, int pos) override;
protected:
private:
};

#endif /* defined(__FootBallX__CFBFormation__) */
