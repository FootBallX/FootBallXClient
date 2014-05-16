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

class CFBMatchProxy
{
public:
    typedef function<void(const vector<float>&, int, unsigned int)> TEAM_POSITION_FUNC;
    typedef function<void(const vector<vector<float>>&, FBDefs::SIDE, FBDefs::SIDE, unsigned int)> START_MATCH_FUNC;
    typedef function<void()> END_MATCH_FUNC;
    typedef function<void(FBDefs::MENU_TYPE, vector<int>&, vector<int>&)> TRIGGER_MENU_FUNC;
    typedef function<void(unsigned int)> INSTRUCTION_ACK_FUNC;
    typedef function<void(const CFBInstructionResult&)> INSTRUCTION_RESULT_FUNC;
    
    CFBMatchProxy() = default;
    virtual ~CFBMatchProxy() = default;
    
    virtual void start() = 0;
    virtual void sendTeamPosition(const vector<float>&, int) = 0;
    virtual void sendMenuCmd(FBDefs::MENU_ITEMS, int) = 0;
    
    virtual void setTeamPositionAck(TEAM_POSITION_FUNC) = 0;
    virtual void setStartMatchAck(START_MATCH_FUNC) = 0;
    virtual void setEndMatchAck(END_MATCH_FUNC) = 0;
    virtual void setTriggerMenuAck(TRIGGER_MENU_FUNC) = 0;
    virtual void setInstructionAck(INSTRUCTION_ACK_FUNC) = 0;
    virtual void setInstructionResultAck(INSTRUCTION_RESULT_FUNC) = 0;
    
    virtual void update(float dt) = 0;
    
    virtual unsigned int getTime() = 0;
    virtual float getDeltaTime(unsigned int) = 0;
};

#endif /* defined(__FootBallX__CFBMatchProxy__) */
