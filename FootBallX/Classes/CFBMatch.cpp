//
//  CFBMatch.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-21.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBMatch.h"
#include "CFBPitch.h"


IMPLEMENT_SINGLETON(CFBMatch);

CFBMatch::CFBMatch()
: m_pitch(nullptr)
, m_ball(nullptr)
{
    
}



CFBMatch::~CFBMatch()
{
    CC_SAFE_DELETE(m_pitch);
    CC_SAFE_DELETE(m_ball);
}



bool CFBMatch::init()
{
    do
    {
        m_pitch = new CFBPitch;
        m_ball = new CFBBall;
        
        return true;
    } while (false);
    
    return false;
}



void CFBMatch::setTeam(FBDefs::SIDE side, CFBTeam* team)
{
    CC_ASSERT(side < FBDefs::SIDE::NONE);
    m_teams[(int)side] = team;
    team->setSide(side);
}



CFBTeam* CFBMatch::getTeam(FBDefs::SIDE side)
{
    CC_ASSERT(side < FBDefs::SIDE::NONE);
    
    return m_teams[(int)side];
}



void CFBMatch::update(float dt)
{
    for (auto x : m_teams)
    {
        x->update(dt);
    }
}



bool CFBMatch::startMatch(FBDefs::SIDE side)
{
    do
    {
        for (auto x : m_teams)
        {
            x->onStartMatch();
        }
        m_teams[(int)side]->kickOff();
        return true;
    } while (false);
    
    return false;
}


CFBTeam* CFBMatch::getPlayingTeam()
{
    for (auto x : m_teams)
    {
        if (x->getPlayingPlayer())
        {
            return x;
        }
    }
    return nullptr;
}



CFBPlayer* CFBMatch::getPlayingPlayer()
{
    for (auto x : m_teams)
    {
        auto player = x->getPlayingPlayer();
        if (player)
        {
            return player;
        }
    }
    return nullptr;
}




bool CFBMatch::isBallOnTheSide(FBDefs::SIDE side)
{
    auto& pos = this->getBall()->getBallPos();
    auto pitch = getPitch();
    if (side == FBDefs::SIDE::LEFT)
    {
        return pos.x < pitch->transformPersentageX(0.5f);
    }
    else
    {
        return pos.x > pitch->transformPersentageX(0.5f);
    }
	return false;
}



float CFBMatch::getBallPosRateBySide(FBDefs::SIDE side)
{
    auto& pos = this->getBall()->getBallPos();
    auto pitch = getPitch();
    float rate = pos.x / pitch->getPitchWidth();
    if (side == FBDefs::SIDE::RIGHT)
    {
        rate = 1 - rate;
    }
    
    return rate;
}



