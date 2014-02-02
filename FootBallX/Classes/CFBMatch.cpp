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
, m_redTeam(nullptr)
, m_blackTeam(nullptr)
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


void CFBMatch::update(float dt)
{
    m_redTeam->update(dt);
    m_blackTeam->update(dt);
}



bool CFBMatch::startMatch(bool redTeamKickoff)
{
    do
    {
        if (redTeamKickoff)
        {
            m_redTeam->setSide(FBDefs::SIDE::LEFT);
            m_blackTeam->setSide(FBDefs::SIDE::RIGHT);
            m_redTeam->onStartMatch();
            m_blackTeam->onStartMatch();
            m_redTeam->kickOff();
            
        }
        else
        {
            m_redTeam->setSide(FBDefs::SIDE::RIGHT);
            m_blackTeam->setSide(FBDefs::SIDE::LEFT);
            m_redTeam->onStartMatch();
            m_blackTeam->onStartMatch();
            m_blackTeam->kickOff();
        }

        return true;
    } while (false);
    
    return false;
}


CFBTeam* CFBMatch::getPlayingTeam()
{
    if (m_redTeam->getPlayingPlayer()) return m_redTeam;
    if (m_blackTeam->getPlayingPlayer()) return m_blackTeam;
    
    return nullptr;
}



CFBPlayer* CFBMatch::getPlayingPlayer()
{
    auto player = m_redTeam->getPlayingPlayer();
    if (player) return player;
    
    player = m_blackTeam->getPlayingPlayer();
    if (player) return player;
    
    return nullptr;
}


