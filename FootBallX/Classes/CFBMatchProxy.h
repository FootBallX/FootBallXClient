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
    typedef function<void(const cocos2d::Point&, const cocos2d::Point&)> PLAYER_MOVE_FUNC;
    typedef function<void(const vector<float>&)> TEAM_POSITION_FUNC;
    typedef function<void(int)> SWITCH_HILIGHT_PLAYER_FUNC;
    typedef function<void(FBDefs::SIDE, FBDefs::SIDE, long long)> START_MATCH_FUNC;
    typedef function<void()> END_MATCH_FUNC;
    
    CFBMatchProxy() = default;
    virtual ~CFBMatchProxy() = default;
    
    virtual void start() = 0;
    virtual void sendPlayerMove(const cocos2d::Point&, const cocos2d::Point&) = 0;
    virtual void sendTeamPosition(const vector<float>&) = 0;
    virtual void sendHiligtPlayer(int) = 0;
    
    virtual void setPlayerMoveAck(PLAYER_MOVE_FUNC) = 0;
    virtual void setTeamPositionAck(TEAM_POSITION_FUNC) = 0;
    virtual void setSwitchHilightPlayerAkc(SWITCH_HILIGHT_PLAYER_FUNC) = 0;
    virtual void setStartMatchAck(START_MATCH_FUNC) = 0;
    virtual void setEndMatchAck(END_MATCH_FUNC) = 0;
    
    virtual void update(float dt) = 0;
    
};

#endif /* defined(__FootBallX__CFBMatchProxy__) */
