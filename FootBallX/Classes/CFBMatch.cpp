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



bool CFBMatch::init(float pitchWidth, float pitchHeight, IFBMatchUI* matchUI, CFBMatchProxy* proxy)
{
    do
    {
        m_pitch = new CFBPitch;
        m_ball = new CFBBall;
        
        m_matchUI = matchUI;
        
        CC_SAFE_DELETE(m_proxy);
        m_proxy = proxy;
        m_proxy->setPlayerMoveAck(std::bind(&CFBMatch::playerMoveAck, this, std::placeholders::_1, std::placeholders::_2));
        m_proxy->setSwitchHilightPlayerAkc(std::bind(&CFBMatch::switchHilightPlayerAkc, this, std::placeholders::_1));
        m_proxy->setEndMatchAck(std::bind(&CFBMatch::endMatchAck, this));
        m_proxy->setTeamPositionAck(std::bind(&CFBMatch::teamPositionAck, this, std::placeholders::_1));
        m_proxy->setStartMatchAck(std::bind(&CFBMatch::startMatchAck, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));
        
        BREAK_IF_FAILED(m_pitch->init(pitchWidth, pitchHeight));
        
        m_playerDistanceSq = FBDefs::PLAYER_DISTANCE * FBDefs::PLAYER_DISTANCE;

        m_matchStep = FBDefs::MATCH_STEP::WAIT_START;
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
    
    m_teamsInMatch[(int)SIDE::SELF] = m_teams[(int)side];
    m_teamsInMatch[(int)SIDE::OPP] = m_teams[(int)m_pitch->getOtherSide(side)];
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



const Point& CFBMatch::getControlSidePlayerMovingVec()
{
    return m_movingVec[(int)SIDE::SELF];
}



CFBTeam* CFBMatch::getOtherTeam(CFBTeam* team)
{
    if (m_teams[0] == team) return m_teams[1];
    else if (m_teams[1] == team) return m_teams[0];
    
    return nullptr;
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
    switch (m_matchStep)
    {
        case FBDefs::MATCH_STEP::WAIT_START:
            m_proxy->update(dt);
            break;
        case FBDefs::MATCH_STEP::MATCHING:
        {
            if (!m_isPause)
            {
                for (auto x : m_teams)
                {
                    x->update(dt);
                }
                
                m_proxy->update(dt);
                
                updateEncounter(dt);
            }
            
            if (m_currentInstruction != nullptr)
            {
                m_currentInstruction->update(dt);
            }
            
            for (int i = 0; i < (int)SIDE::NONE; ++i)
            {
                auto& vec = m_movingVec[i];
                bool vecValid = false;
                if (vec.x != 0 || vec.y != 0)
                {
                    vecValid = true;
                    auto hilightPlayer = m_teamsInMatch[i]->getHilightPlayer();
                    auto pos = hilightPlayer->getPosition();
                    pos += vec * hilightPlayer->getSpeed() * dt;
                    hilightPlayer->setPosition(pos);
                }
                
                if (m_syncTime[i] < 0)
                {
                    if (i == (int)SIDE::SELF)
                    {
                        m_syncTime[i] = m_SYNC_TIME;
                        if (vecValid)
                        {
                            auto team = getControlSideTeam();
                            auto player = team->getHilightPlayer();
                            m_proxy->sendPlayerMove(player->getPosition(), vec);
                        }
                    }
                    else
                    {
                        vec.setPoint(0, 0);
                    }
                }
                
                m_syncTime[i] -= dt;
            }
            break;
        }
    }
}



bool CFBMatch::startMatch()
{
    m_proxy->start();
    
    return true;
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




void CFBMatch::pauseGame(bool p)
{
    m_isPause = p;

    m_matchUI->onPauseGame(p);
    
    for (int i = 0; i < (int)SIDE::NONE; ++i)
    {
        m_movingVec[i].setPoint(0, 0);
    }
    syncTeam();
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
    
    m_matchUI->onInstrunctionEnd();
    
    checkEncounterInPenaltyArea();
}



void CFBMatch::playAnimation(const string& name, float delay)
{
    m_matchUI->onPlayAnimation(name, delay);
}



void CFBMatch::onAnimationEnd()
{
    if (m_currentInstruction)
    {
        m_currentInstruction->onAnimationEnd();
    }
}



FBDefs::MATCH_STEP CFBMatch::getMatchStep()
{
    return m_matchStep;
}

#pragma mark - encounter

void CFBMatch::updateEncounter(float dt)
{
    return;
    m_encounterTime -= dt;
    
    if (m_menuType != FBDefs::MENU_TYPE::NONE)
    {
        if (m_encounterTime < 0)
        {
            m_encounterTime = FLT_MAX;
            
            m_matchUI->onMenu(m_menuType, m_isAir, m_involvePlayerIds);
            
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



#pragma mark - net or sim

void CFBMatch::setBallControllerMove(const Point& vec, bool syncImmediately)
{
    m_movingVec[(int)SIDE::SELF] = vec;
    
    if (syncImmediately)
    {
        auto team = getControlSideTeam();
        auto player = team->getHilightPlayer();
        m_proxy->sendPlayerMove(player->getPosition(), vec);
    }
}



void CFBMatch::syncHilightPlayer()
{
    m_proxy->sendHiligtPlayer(m_teamsInMatch[(int)SIDE::OPP]->getHilightPlayerId());
}



void CFBMatch::syncTeam()
{
    auto team = m_teamsInMatch[(int)SIDE::OPP];
    auto fmt = team->getFormation();
    
    vector<float> v;
    for (int i = 0; i < fmt->getPlayerNumber(); ++i)
    {
        auto player = fmt->getPlayer(i);
        auto& pos = player->getPosition();
        v.push_back(pos.x);
        v.push_back(pos.y);
    }
    
    m_proxy->sendTeamPosition(v);
}



void CFBMatch::playerMoveAck(const Point& pos, const Point& vec)
{
    m_movingVec[(int)SIDE::OPP] = vec;
    
    auto team = getControlSideTeam();
    team= getOtherTeam(team);
    team->getHilightPlayer()->setPosition(pos);
    
    m_syncTime[(int)SIDE::OPP] = m_SYNC_TIME;
}



void CFBMatch::teamPositionAck(const vector<float>& p)
{
    auto side = getControlSide();
    auto otherSide = getPitch()->getOtherSide(side);
    auto team = getTeam(otherSide);
    auto fmt = team->getFormation();
    
    int size = fmt->getPlayerNumber();
    
    for (int i = 0; i < size; ++i)
    {
        Point pos(p[i * 2], p[i * 2 + 1]);
        auto player = fmt->getPlayer(i);
        player->setPosition(pos);
    }
}



void CFBMatch::switchHilightPlayerAkc(int playerId)
{
    auto team = getControlSideTeam();
    team= getOtherTeam(team);
    team->setHilightPlayerId(playerId);
}



void CFBMatch::startMatchAck(FBDefs::SIDE mySide, FBDefs::SIDE kickOffSide, long long st)
{
    setControlSide(mySide);
    for (auto x : m_teams)
    {
        x->onStartMatch();
    }
    
    m_teams[(int)kickOffSide]->kickOff();
    
    m_matchStep = FBDefs::MATCH_STEP::MATCHING;
}



void CFBMatch::endMatchAck()
{
    m_matchUI->onGameEnd();
}




