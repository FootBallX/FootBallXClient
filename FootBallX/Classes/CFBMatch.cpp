//
//  CFBMatch.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-21.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBMatch.h"
#include "CFBPitch.h"
#include "CFBFunctionsJS.h"
#include "CFBInstruction.h"

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
    if (!m_isPause)
    {
        for (auto x : m_teams)
        {
            x->update(dt);
        }
    }
    
    if (m_currentInstruction != nullptr)
    {
        m_currentInstruction->update(dt);
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



void CFBMatch::setOnAtkMenuCallback(function<void(const vector<int>&)> cb)
{
    m_onAtkMenu = cb;
}



void CFBMatch::setOnDefMenuCallback(function<void(const vector<int>&)> cb)
{
    m_onDefMenu = cb;
}



void CFBMatch::setOnPlayAnimationCallback(function<void(const string& name, float delay)> cb)
{
    m_onPlayAnimation = cb;
}



void CFBMatch::pauseGame(bool p)
{
    m_isPause = p;
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



void CFBMatch::tryPassBall(CFBPlayer* from, CFBPlayer* to)
{
    CC_ASSERT(from->m_ownerTeam == to->m_ownerTeam);
    auto& team = from->m_ownerTeam;
    auto size = team->getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(size);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto& otherTeamMembers = otherTeam->getTeamMembers();
    
    m_currentInstruction = INS_FAC->getPassBallIns();
    m_currentInstruction->addPlayer(from);
    
    vector<pair<float, CFBPlayer*>> involvePlayers;
    
    for (auto player : otherTeamMembers)
    {
        if (player->m_isOnDuty && !player->m_isGoalKeeper)
        {
            if (FBDefs::isPointOnTheWay(from->m_curPosition, to->m_curPosition, player->m_curPosition))
            {
                float dist = from->m_curPosition.getDistanceSq(player->m_curPosition);
                involvePlayers.push_back(pair<float, CFBPlayer*>(dist, player));
            }
        }
    }
    
    m_eventState = FBDefs::MATCH_EVENT_STATE::PASS_BALL;
    std::sort(involvePlayers.begin(), involvePlayers.end(),
              [&](const pair<float, CFBPlayer*>& o1, const pair<float, CFBPlayer*> o2)-> bool
              {
                  return o1.first < o2.first;
              });
    
    for (auto a : involvePlayers)
    {
        m_currentInstruction->addPlayer(a.second);
    }
    
    m_currentInstruction->addPlayer(to);
    
    m_currentInstruction->start(bind(&CFBMatch::onInstructionEnd, this));
    
    pauseGame(true);
}



//void CFBMatch::updatePassBall(float dt)
//{
//    if (m_involvePlayers.size() == 0)
//    {
//        m_eventFromRole->m_ownerTeam->passBall(m_eventToRole->m_positionInFormation);
//    }
//    else
//    {
//        for (auto p : m_involvePlayers)
//        {
//            auto player = p.second;
//            float rate = FB_FUNC_JS->compute(m_eventFromRole->getPlayerCard(), player->getPlayerCard());
//            if (rate < 50)
//            {
//                m_eventFromRole->m_ownerTeam->passBall(m_eventToRole->m_positionInFormation);
//                break;
//            }
//        }
//    }
//}



void CFBMatch::onInstructionEnd()
{
    pauseGame(false);
    m_currentInstruction = nullptr;
}



void CFBMatch::playAnimation(const string& name, float delay)
{
    if (m_onPlayAnimation)
    {
        m_onPlayAnimation(name, delay);
    }
}


