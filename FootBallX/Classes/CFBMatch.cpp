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



bool CFBMatch::init(float pitchWidth, float pitchHeight)
{
    do
    {
        m_pitch = new CFBPitch;
        m_ball = new CFBBall;
        
        BREAK_IF_FAILED(m_pitch->init(pitchWidth, pitchHeight));
        
        m_playerDistanceSq = m_pitch->transformPersentageX(FBDefs::PLAYER_DISTANCE);
        m_playerDistanceSq *= m_playerDistanceSq;
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
    
    checkEncounter(dt);
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



void CFBMatch::setOnAtkMenuCallback(function<void(const vector<int>&)> cb)
{
    m_onAtkMenu = cb;
}



void CFBMatch::setOnDefMenuCallback(function<void(const vector<int>&)> cb)
{
    m_onDefMenu = cb;
}



bool CFBMatch::checkEncounter(float dt)
{
    m_encounterTime -= dt;
    if (m_encounterTime < 0)
    {
        m_encounterTime = FLT_MAX;
        if (m_onAtkMenu)
        {
            m_onAtkMenu(m_defendPlayerIds);
        }
    }
    
    m_defendPlayerIds.clear();
    
    auto ball = getBall();
    auto side = ball->m_ownerTeam->getSide();
    auto pitch = getPitch();
    
    auto otherSide = pitch->getOtherSide(side);
    auto ballPos = ball->getBallPos();
    
    auto defTeam = getTeam(otherSide);
    auto& teamMembers = defTeam->getTeamMembers();
    
    for (auto tm : teamMembers)
    {
        if (tm->m_isOnDuty && !tm->m_isGoalKeeper)
        {
            if (FLT_LE(ballPos.getDistanceSq(tm->m_curPosition), m_playerDistanceSq))
            {
                m_defendPlayerIds.push_back(tm->m_positionInFormation);
            }
        }
    }
    
    auto size = m_defendPlayerIds.size();
    if (size > 0)
    {
        if (size >= 4)
        {
            m_encounterTime = -1;
        }
        else if (m_encounterTime > FBDefs::PLAYER_ENCOUNTER_TRIGGER_TIME)
        {
            m_encounterTime = FBDefs::PLAYER_ENCOUNTER_TRIGGER_TIME;
        }
        return true;
    }
    else
    {
        m_encounterTime = FLT_MAX;
    }
    
    return false;
}



