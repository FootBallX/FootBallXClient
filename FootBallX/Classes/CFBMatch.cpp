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
#include "CRandomManager.h"
#include "CFBPassBallIns.h"
#include "CFBShootBallAirIns.h"
#include "CFBShootBallGroundIns.h"

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
        
        m_playerDistanceSq = FBDefs::PLAYER_DISTANCE * FBDefs::PLAYER_DISTANCE;

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



void CFBMatch::setControlSide(FBDefs::SIDE side)
{
    CC_ASSERT(side != FBDefs::SIDE::NONE);
    m_controlSide = side;
}



bool CFBMatch::checkControlSide(FBDefs::SIDE side)
{
    return m_controlSide == side;
}



FBDefs::SIDE CFBMatch::getControlSide()
{
    return m_controlSide;
}



CFBTeam* CFBMatch::getControlSideTeam()
{
    return getTeam(m_controlSide);
}



CFBTeam* CFBMatch::getTeam(FBDefs::SIDE side)
{
    CC_ASSERT(side < FBDefs::SIDE::NONE);
    
    return m_teams[(int)side];
}



CFBTeam* CFBMatch::getAttackingTeam()
{
    auto team = getTeam(FBDefs::SIDE::LEFT);
    auto player = team->getHilightPlayer();
    if (player && player->m_isBallController)
    {
        return team;
    }
    
    team = getTeam(FBDefs::SIDE::RIGHT);
    player = team->getHilightPlayer();
    if (player && player->m_isBallController)
    {
        return team;
    }
    
    CC_ASSERT(false);
    return nullptr;
}



CFBTeam* CFBMatch::getDefendingTeam()
{
    auto team = getTeam(FBDefs::SIDE::LEFT);
    auto player = team->getHilightPlayer();
    if (!player || !player->m_isBallController)
    {
        return team;
    }
    
    team = getTeam(FBDefs::SIDE::RIGHT);
    player = team->getHilightPlayer();
    if (!player || !player->m_isBallController)
    {
        return team;
    }
    
    CC_ASSERT(false);
    return nullptr;
}



void CFBMatch::update(float dt)
{
    if (!m_isPause)
    {
        for (auto x : m_teams)
        {
            x->update(dt);
        }
        
        updateEncounter(dt);
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



bool CFBMatch::isBallOnTheSide(FBDefs::SIDE side)
{
    auto& pos = this->getBallPosition();
    auto pitch = getPitch();
    if (side == FBDefs::SIDE::LEFT)
    {
        return pos.x < pitch->getPitchWidth() * 0.5f;
    }
    else
    {
        return pos.x > pitch->getPitchWidth() * 0.5f;
    }
	return false;
}



float CFBMatch::getBallPosRateBySide(FBDefs::SIDE side)
{
    auto& pos = this->getBallPosition();
    auto pitch = getPitch();
    float rate = pos.x / pitch->getPitchWidth();
    if (side == FBDefs::SIDE::RIGHT)
    {
        rate = 1 - rate;
    }
    
    return rate;
}



void CFBMatch::setBallPosition(const Point& pos)
{
    m_ball->setBallPos(pos);
}



const Point& CFBMatch::getBallPosition()
{
    return m_ball->getBallPos();
}



void CFBMatch::setOnMenuCallback(function<void(FBDefs::MENU_TYPE, bool, const vector<int>&)> cb)
{
    m_onMenu = cb;
}



void CFBMatch::setOnPlayAnimationCallback(function<void(const string& name, float delay)> cb)
{
    m_onPlayAnimation = cb;
}



void CFBMatch::setOnInstructionEnd(function<void(void)> cb)
{
    m_onInstructionEnd = cb;
}



void CFBMatch::setOnPauseGame(function<void(bool)> cb)
{
    m_onPauseGame = cb;
}


void CFBMatch::pauseGame(bool p)
{
    m_isPause = p;
    if (m_onPauseGame)
    {
        m_onPauseGame(p);
    }
}



void CFBMatch::updateDefendPlayerAroundBall()
{
    m_defendPlayerIds.clear();
    
    auto ballPos = FBMATCH->getBallPosition();
    
    auto defTeam = getDefendingTeam();
    auto& teamMembers = defTeam->getTeamMembers();
    
    for (auto tm : teamMembers)
    {
        if (tm->m_isOnDuty && !tm->m_isGoalKeeper)
        {
            if (FLT_LE(ballPos.getDistanceSq(tm->getPosition()), m_playerDistanceSq))
            {
                m_defendPlayerIds.insert(tm->m_positionInFormation);
            }
        }
    }
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
    
    
    for (auto x : m_defendPlayerIds)
    {
        auto pO = otherTeam->getFormation()->getPlayer(x);
        int roll = RANDOM_MGR->getRand() % 300;
        if (roll > 200)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::TAKCLE);
        }
        else if (roll > 100)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::INTERCEPT);
        }
        else
        {
            pO->setInstruction(FBDefs::PLAYER_INS::BLOCK);
        }
        m_currentInstruction->addPlayer(pO);
    }
    
    auto& fpos = from->getPosition();
    auto& tpos = to->getPosition();
    
    vector<pair<float, CFBPlayer*>> involvePlayers;
    
    for (auto player : otherTeamMembers)
    {
        if (player->m_isOnDuty && !player->m_isGoalKeeper)
        {
            auto it = std::find(m_defendPlayerIds.begin(), m_defendPlayerIds.end(), player->m_positionInFormation);
            if (it == m_defendPlayerIds.end())
            {
                auto& ppos = player->getPosition();
                if (FBDefs::isPointOnTheWay(fpos, tpos, ppos))
                {
                    float dist = fpos.getDistanceSq(ppos);
                    involvePlayers.push_back(pair<float, CFBPlayer*>(dist, player));
                }
            }
        }
    }
    
    std::sort(involvePlayers.begin(), involvePlayers.end(),
              [&](const pair<float, CFBPlayer*>& o1, const pair<float, CFBPlayer*> o2)-> bool
              {
                  return o1.first < o2.first;
              });
    
    for (auto a : involvePlayers)
    {
        auto pO = a.second;
        int roll = RANDOM_MGR->getRand() % 300;
        if (roll > 200)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::TAKCLE);
        }
        else if (roll > 100)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::INTERCEPT);
        }
        else
        {
            pO->setInstruction(FBDefs::PLAYER_INS::BLOCK);
        }
        m_currentInstruction->addPlayer(pO);
    }
    
    m_currentInstruction->addPlayer(to);
    
    m_currentInstruction->start(bind(&CFBMatch::onInstructionEnd, this));
    
    pauseGame(true);
}



void CFBMatch::tryShootBall(CFBPlayer* shooter, bool isAir)
{
    auto& team = shooter->m_ownerTeam;
    auto size = team->getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(size);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto& otherTeamMembers = otherTeam->getTeamMembers();
    Point goalPos = pitch->getGoalPos(otherSide);
    
    if (isAir)
    {
        m_currentInstruction = INS_FAC->getShootBallAirIns();
    }
    else
    {
        m_currentInstruction = INS_FAC->getShootBallGroundIns();
    }

    m_currentInstruction->addPlayer(shooter);
    
    for (auto x : m_defendPlayerIds)
    {
        auto pO = otherTeam->getFormation()->getPlayer(x);
        int roll = RANDOM_MGR->getRand() % 300;
        if (roll > 200)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::TAKCLE);
        }
        else if (roll > 100)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::INTERCEPT);
        }
        else
        {
            pO->setInstruction(FBDefs::PLAYER_INS::BLOCK);
        }
        m_currentInstruction->addPlayer(pO);
    }
    
    auto& fpos = shooter->getPosition();
    
    vector<pair<float, CFBPlayer*>> involvePlayers;
    
    CFBPlayer* goalKeeper = nullptr;
    for (auto player : otherTeamMembers)
    {
        if (player->m_isOnDuty)
        {
            if (player->m_isGoalKeeper)
            {
                goalKeeper = player;
            }
            else
            {
                auto it = std::find(m_defendPlayerIds.begin(), m_defendPlayerIds.end(), player->m_positionInFormation);
                if (it == m_defendPlayerIds.end())
                {
                    auto& ppos = player->getPosition();
                    if (FBDefs::isPointOnTheWay(fpos, goalPos, ppos))
                    {
                        float dist = fpos.getDistanceSq(ppos);
                        involvePlayers.push_back(pair<float, CFBPlayer*>(dist, player));
                    }
                }
            }
        }
    }
    
    std::sort(involvePlayers.begin(), involvePlayers.end(),
              [&](const pair<float, CFBPlayer*>& o1, const pair<float, CFBPlayer*> o2)-> bool
              {
                  return o1.first < o2.first;
              });
    
    for (auto a : involvePlayers)
    {
        auto pO = a.second;
        int roll = RANDOM_MGR->getRand() % 300;
        if (roll > 200)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::TAKCLE);
        }
        else if (roll > 100)
        {
            pO->setInstruction(FBDefs::PLAYER_INS::INTERCEPT);
        }
        else
        {
            pO->setInstruction(FBDefs::PLAYER_INS::BLOCK);
        }
        m_currentInstruction->addPlayer(pO);
    }
    
    CC_ASSERT(goalKeeper);
    int roll = RANDOM_MGR->getRand() % 1000;
    if (roll > 500)
    {
        goalKeeper->setInstruction(FBDefs::PLAYER_INS::HIT);
    }
    else
    {
        goalKeeper->setInstruction(FBDefs::PLAYER_INS::TAKE);
    }
    m_currentInstruction->addPlayer(goalKeeper);
    
    m_currentInstruction->start(bind(&CFBMatch::onInstructionEnd, this));
    
    pauseGame(true);

}



void CFBMatch::onInstructionEnd()
{
    pauseGame(false);
    
    m_recentEndedFlow = m_currentInstruction->getInstructionType();
    m_currentInstruction = nullptr;
    
    if (m_onInstructionEnd)
    {
        m_onInstructionEnd();
    }
    
    checkEncounterInPenaltyArea();
}



void CFBMatch::playAnimation(const string& name, float delay)
{
    if (m_onPlayAnimation)
    {
        m_onPlayAnimation(name, delay);
    }
}



void CFBMatch::onAnimationEnd()
{
    if (m_currentInstruction)
    {
        m_currentInstruction->onAnimationEnd();
    }
}


#pragma mark - encounter

void CFBMatch::updateEncounter(float dt)
{
    m_encounterTime -= dt;
    
    if (m_menuType != FBDefs::MENU_TYPE::NONE)
    {
        if (m_encounterTime < 0)
        {
            m_encounterTime = FLT_MAX;
            if (m_onMenu)
            {
                m_onMenu(m_menuType, m_isAir, m_involvePlayerIds);
            }
            
            m_menuType = FBDefs::MENU_TYPE::NONE;
        }
    }
    else
    {
        updateDefendPlayerAroundBall();
        checkEncounterInDribble();
        checkEncounterInPenaltyArea();
        
        m_recentEndedFlow = FBDefs::MATCH_FLOW_TYPE::NONE;
    }
}



void CFBMatch::checkEncounterInDribble()
{
    if (m_menuType != FBDefs::MENU_TYPE::NONE) return;
    
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
        
        auto team = getControlSideTeam();
        if (team->isAttacking())
        {
            m_menuType = FBDefs::MENU_TYPE::ENCOUNTER_ATK_G;
            m_involvePlayerIds.clear();
            m_involvePlayerIds.push_back(team->getHilightPlayerId());
        }
        else
        {
            m_menuType = FBDefs::MENU_TYPE::ENCOUTNER_DEF_G;
            m_involvePlayerIds.clear();
            for (auto p : m_defendPlayerIds)
            {
                m_involvePlayerIds.push_back(p);
            }
        }
        m_isAir = false;
    }
    else
    {
        m_encounterTime = FLT_MAX;
        
        m_menuType = FBDefs::MENU_TYPE::NONE;
    }
}


// 进攻方对方禁区拿球，强制触发一次空中遭遇。
void CFBMatch::checkEncounterInPenaltyArea()
{
    // TODO: 仅当传球，二过一，随机球 刚刚结束，否则直接返回。二过一和随机球还未做判断
    if (m_recentEndedFlow != FBDefs::MATCH_FLOW_TYPE::PASSBALL) return;
    
    if (m_menuType != FBDefs::MENU_TYPE::NONE) return;
    
    auto team = this->getAttackingTeam();
    auto side = team->getSide();
    auto ballPos = getBallPosition();
    if (m_pitch->isInPenaltyArea(ballPos, m_pitch->getOtherSide(side)))
    {
        m_encounterTime = -1;
        m_isAir = true;
        
        auto conTeam = getControlSideTeam();
        if (conTeam->isAttacking())
        {
            m_menuType = FBDefs::ENCOUNTER_ATK_OPPSITE_A;
            m_involvePlayerIds.clear();
            m_involvePlayerIds.push_back(conTeam->getHilightPlayerId());
        }
        else
        {
            m_menuType = FBDefs::ENCOUNTER_DEF_SELF_A;
            m_involvePlayerIds.clear();
            for (auto p : m_defendPlayerIds)
            {
                m_involvePlayerIds.push_back(p);
            }
        }
    }
}

