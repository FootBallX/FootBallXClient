//
//  CFBMatchProxy.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBMatchProxy__
#define __FootBallX__CFBMatchProxy__

#include "Common.h"
#include "FBDefs.h"
#include "CFBInstructionResult.h"
#include "CFBPlayerInitInfo.h"

class IMatchProxyDelegator
{
public:
    virtual void syncTeam() = 0;
    virtual void teamPositionAck(int side, const vector<float>&, int, unsigned int) = 0;
    virtual void matchInfoAck(FBDefs::SIDE, FBDefs::SIDE, int) = 0;
    virtual void startMatchAck(unsigned int) = 0;
    virtual void endMatchAck() = 0;
    virtual void triggerMenuAck(FBDefs::MENU_TYPE, vector<int>&, vector<int>&) = 0;
    virtual void instructionAck(unsigned int) = 0;
    virtual void instructionResultAck() = 0;
    virtual void addPlayer(FBDefs::SIDE, const CFBPlayerInitInfo&) = 0;
    virtual void resumeMatch() = 0;
    
    virtual CFBInstructionResult& getInstructionResult() = 0;
};

class CFBMatchProxy
{
public:
    CFBMatchProxy() = default;
    virtual ~CFBMatchProxy() = default;
    
    virtual void setDelegator(IMatchProxyDelegator*) = 0;
    virtual void start() = 0;
    virtual void sendTeamPosition(const vector<float>&, int, int) = 0;
    virtual void sendMenuCmd(FBDefs::MENU_ITEMS, int) = 0;
    virtual void sendInstructionMovieEnd() = 0;
    
    virtual void update(float dt) = 0;
    
    virtual unsigned int getTime() = 0;
    virtual float getDeltaTime(unsigned int) = 0;
};

#endif /* defined(__FootBallX__CFBMatchProxy__) */
