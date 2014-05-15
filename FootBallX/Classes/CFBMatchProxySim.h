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
    virtual void sendTeamPosition(const vector<float>&, int) override;
    virtual void sendMenuCmd(FBDefs::MENU_ITEMS) override;
    
    virtual void setTeamPositionAck(TEAM_POSITION_FUNC) override;
    virtual void setStartMatchAck(START_MATCH_FUNC) override;
    virtual void setEndMatchAck(END_MATCH_FUNC) override;
    virtual void setTriggerMenuAck(TRIGGER_MENU_FUNC) override;
    virtual void setInstructionAck(INSTRUCTION_ACK_FUNC) override;
    
    virtual void update(float dt) override;
    
    virtual unsigned int getTime() override;
    virtual float getDeltaTime(unsigned int) override;
protected:
    TEAM_POSITION_FUNC m_teamPositionAck;
    START_MATCH_FUNC m_startMatchAck;
    END_MATCH_FUNC m_endMatchAck;
    TRIGGER_MENU_FUNC m_triggerMenuAck;
    INSTRUCTION_ACK_FUNC m_instructionAck;
};

#endif /* defined(__FootBallX__CFBMatchProxySim__) */
