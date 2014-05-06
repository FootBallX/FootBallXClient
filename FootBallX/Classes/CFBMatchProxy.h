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

class CFBMatchProxy
{
public:
    typedef function<void(const vector<float>&, int, long long)> TEAM_POSITION_FUNC;
    typedef function<void(FBDefs::SIDE, FBDefs::SIDE, long long)> START_MATCH_FUNC;
    typedef function<void()> END_MATCH_FUNC;
    typedef function<void(FBDefs::MENU_TYPE, vector<int>&, vector<int>&)> TRIGGER_MENU_FUNC;
    
    CFBMatchProxy() = default;
    virtual ~CFBMatchProxy() = default;
    
    virtual void start() = 0;
    virtual void sendTeamPosition(const vector<float>&, int) = 0;
    virtual void sendMenuCmd(FBDefs::MENU_ITEMS) = 0;
    
    virtual void setTeamPositionAck(TEAM_POSITION_FUNC) = 0;
    virtual void setStartMatchAck(START_MATCH_FUNC) = 0;
    virtual void setEndMatchAck(END_MATCH_FUNC) = 0;
    virtual void setTriggerMenuAck(TRIGGER_MENU_FUNC) = 0;
    
    virtual void update(float dt) = 0;
    
    virtual long long getTime() = 0;
    virtual float getDeltaTime(long long) = 0;
};

#endif /* defined(__FootBallX__CFBMatchProxy__) */
