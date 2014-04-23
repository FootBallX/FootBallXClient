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
        m_startMatchAck(FBDefs::SIDE::LEFT, FBDefs::SIDE::LEFT, 0);
    }
}



void CFBMatchProxySim::sendTeamPosition(const vector<float>&, int)
{
    // do nothing.
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



