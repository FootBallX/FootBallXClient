//
//  CFBMatchProxySim.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBMatchProxySim.h"
#include "CFBMatch.h"

CFBMatchProxySim::CFBMatchProxySim()
{
}



CFBMatchProxySim::~CFBMatchProxySim()
{
}



void CFBMatchProxySim::start()
{
    if( m_startMatchAck)
    {
//        m_startMatchAck(FBDefs::SIDE::LEFT, FBDefs::SIDE::LEFT, 0);
    }
}



void CFBMatchProxySim::sendTeamPosition(const vector<float>&, int)
{
    // do nothing.
}



void CFBMatchProxySim::sendMenuCmd(FBDefs::MENU_ITEMS, int)
{
    
}



void CFBMatchProxySim::setTeamPositionAck(TEAM_POSITION_FUNC f)
{
    m_teamPositionAck = f;
}



void CFBMatchProxySim::setStartMatchAck(START_MATCH_FUNC f)
{
    m_startMatchAck = f;
}



void CFBMatchProxySim::setEndMatchAck(END_MATCH_FUNC f)
{
    m_endMatchAck = f;
}



void CFBMatchProxySim::setTriggerMenuAck(TRIGGER_MENU_FUNC f)
{
    m_triggerMenuAck = f;
}


void CFBMatchProxySim::setInstructionAck(INSTRUCTION_ACK_FUNC f)
{
    m_instructionAck = f;
}



void CFBMatchProxySim::setInstructionResultAck(INSTRUCTION_RESULT_FUNC f)
{
    m_instructionResultAck = f;
}




void CFBMatchProxySim::update(float dt)
{
    auto team = FBMATCH->getControlSideTeam();
    team = FBMATCH->getOtherTeam(team);
    auto fmt = team->getFormation();
    auto playerId = team->getHilightPlayerId();
    auto playerAI = fmt->getPlayerAIById(playerId);

    playerAI->thinkControlBall();
    playerAI->updateAIControlBall(dt);
}




unsigned int CFBMatchProxySim::getTime()
{
    return 0;
}



float CFBMatchProxySim::getDeltaTime(unsigned int time)
{
    return 0;
}