//
//  CFBMatchProxySim.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBMatchProxySim__
#define __FootBallX__CFBMatchProxySim__

#include "CFBMatchProxy.h"

class CFBMatchProxySim : public CFBMatchProxy
{
public:
    CFBMatchProxySim();
    virtual ~CFBMatchProxySim();
    
    virtual void start() override;
    virtual void sendPlayerMove(const Point&, const Point&) override;
    virtual void sendTeamPosition(const vector<float>&) override;
    virtual void sendHiligtPlayer(int) override;
    
    virtual void setPlayerMoveAck(PLAYER_MOVE_FUNC) override;
    virtual void setTeamPositionAck(TEAM_POSITION_FUNC) override;
    virtual void setSwitchHilightPlayerAkc(SWITCH_HILIGHT_PLAYER_FUNC) override;
    virtual void setStartMatchAck(START_MATCH_FUNC) override;
    virtual void setEndMatchAck(END_MATCH_FUNC) override;
    
    virtual void update(float dt) override;
    
protected:
    PLAYER_MOVE_FUNC m_playerMoveAck;
    TEAM_POSITION_FUNC m_teamPositionAck;
    SWITCH_HILIGHT_PLAYER_FUNC m_switchHilightPlayerAck;
    START_MATCH_FUNC m_startMatchAck;
    END_MATCH_FUNC m_endMatchAck;
};

#endif /* defined(__FootBallX__CFBMatchProxySim__) */
