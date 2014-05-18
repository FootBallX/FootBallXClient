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



void CFBMatchProxySim::setDelegator(IMatchProxyDelegator*)
{
    
}

void CFBMatchProxySim::start()
{
}



void CFBMatchProxySim::sendTeamPosition(const vector<float>&, int, int)
{
    // do nothing.
}



void CFBMatchProxySim::sendMenuCmd(FBDefs::MENU_ITEMS, int)
{
    
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