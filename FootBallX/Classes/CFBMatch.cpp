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
    m_teams[0] = nullptr;
    m_teams[1] = nullptr;
}



CFBMatch::~CFBMatch()
{
    CC_SAFE_DELETE(m_pitch);
    CC_SAFE_DELETE(m_ball);
    CC_SAFE_DELETE(m_teams[0]);
    CC_SAFE_DELETE(m_teams[1]);
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
        m_proxy->setDelegator(this);
        
        BREAK_IF_FAILED(m_pitch->init(pitchWidth, pitchHeight));
        
        m_teams[0] = new CFBTeam(FBDefs::SIDE::LEFT);
        m_teams[1] = new CFBTeam(FBDefs::SIDE::RIGHT);
        
        m_playerDistanceSq = FBDefs::PLAYER_DISTANCE * FBDefs::PLAYER_DISTANCE;

        m_matchStep = FBDefs::MATCH_STEP::WAIT_START;
        return true;
    } while (false);
    
    return false;
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
    m_proxy->update(dt);
    switch (m_matchStep)
    {
        case FBDefs::MATCH_STEP::WAIT_START:
            break;
        case FBDefs::MATCH_STEP::COUNT_DOWN:
        {
            int delta = (int)(m_startTime - m_proxy->getTime());
            if (delta <= 0)
            {
                m_matchStep = FBDefs::MATCH_STEP::MATCHING;
            }
            break;
        }
        case FBDefs::MATCH_STEP::MATCHING:
        {
            if (!m_isPause)
            {
                m_teamsInMatch[(int)SIDE::SELF]->update(dt);
                m_teamsInMatch[(int)SIDE::OPP]->update(dt);
            }
            
            if (m_currentInstruction != nullptr)
            {
                m_currentInstruction->update(dt);
            }
            
            for (int i = 0; i < (int)SIDE::NONE; ++i)
            {
                if (m_syncTime[i] < 0)
                {
                    if (i == (int)SIDE::SELF)
                    {
                        auto hilightPlayer = m_teamsInMatch[(int)SIDE::SELF]->getHilightPlayer();
                        hilightPlayer->setMovingVector(m_vecFromUser);
                        m_teamsInMatch[(int)SIDE::SELF]->think();
                        syncTeam();
                        m_syncTime[i] = m_SYNC_TIME;
                    }
                    else
                    {
                        auto oppTeam = m_teamsInMatch[(int)SIDE::OPP];
                        int num = oppTeam->getPlayerNumber();
                        for (int i = 0; i < num; ++i)
                        {
                            oppTeam->getPlayer(i)->setMovingVector(0, 0);
                        }
                    }
                }
                
                m_syncTime[i] -= dt;
            }
            break;
        }
        case FBDefs::MATCH_STEP::PLAY_ANIM:
        {
            break;
        }
        default:
            break;
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
}



void CFBMatch::updateDefendPlayerAroundBall()
{
    m_defendPlayerIds.clear();
    
    auto ballPos = FBMATCH->getBallPosition();
    
    auto defTeam = getDefendingTeam();
    auto& teamMembers = defTeam->getTeamMembers();
    
    for (auto tm : teamMembers)
    {
        if (!tm->m_isGoalKeeper)
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
    CC_ASSERT(from->getOwnerTeam() == to->getOwnerTeam());
    auto team = from->getOwnerTeam();
    auto size = team->getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(size);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto& otherTeamMembers = otherTeam->getTeamMembers();
    
    m_currentInstruction = INS_FAC->getPassBallIns();
    m_currentInstruction->addPlayer(from);
    
    for (auto x : m_defendPlayerIds)
    {
        auto pO = otherTeam->getPlayer(x);
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
        if (!player->m_isGoalKeeper)
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
              [](const pair<float, CFBPlayer*>& o1, const pair<float, CFBPlayer*> o2)-> bool
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
    auto team = shooter->getOwnerTeam();
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
        auto pO = otherTeam->getPlayer(x);
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
//    if (m_currentInstruction)
//    {
//        m_currentInstruction->onAnimationEnd();
//    }
    
    if (FBDefs::MATCH_STEP::PLAY_ANIM == m_matchStep)
    {
        ++m_playAnimIndex;
        playAnimInInstructionsResult();
    }
}



FBDefs::MATCH_STEP CFBMatch::getMatchStep()
{
    return m_matchStep;
}



int CFBMatch::getCountDownTime()
{
    return (int)-m_proxy->getDeltaTime(m_startTime);
}



unsigned int CFBMatch::getTime()
{
    return m_proxy->getTime();
}



void CFBMatch::playAnimInInstructionsResult()
{
    if (m_playAnimIndex < m_instructionResult.instructions.size())
    {
        auto ins = m_instructionResult.instructions[m_playAnimIndex];
        
        for (auto ani : ins.animations)
        {
            m_matchUI->onPlayAnimation(FBDefs::g_aniNames[ani.aniId], ani.delay);
        }
    }
    else
    {
        // TODO: 恢复正常比赛
    }
}


#pragma mark - Instructions

void CFBMatch::setMenuItem(FBDefs::MENU_ITEMS mi, int targetPlayer)
{
    m_playerInstructions.push_back(mi);
    
    m_proxy->sendMenuCmd(mi, targetPlayer);
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
            
//            m_matchUI->onMenu(m_menuType, m_involvePlayerIds);
            
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
            m_menuType = FBDefs::MENU_TYPE::ENCOUNTER_ATK_OPPSITE_A;
            m_involvePlayerIds.clear();
            m_involvePlayerIds.push_back(conTeam->getHilightPlayerId());
        }
        else
        {
            m_menuType = FBDefs::MENU_TYPE::ENCOUNTER_DEF_SELF_A;
            m_involvePlayerIds.clear();
            for (auto p : m_defendPlayerIds)
            {
                m_involvePlayerIds.push_back(p);
            }
        }
    }
}



void CFBMatch::setBallControllerMove(const Point& vec)
{
    m_vecFromUser = vec;
//    auto hilightPlayer = m_teamsInMatch[(int)SIDE::SELF]->getHilightPlayer();
//    hilightPlayer->setMovingVector(vec);
}

#pragma mark - net or sim

void CFBMatch::syncTeam()
{
    auto team = m_teamsInMatch[(int)SIDE::SELF];
    
    vector<float> v;
    int num = team->getPlayerNumber();
    
    for (int i = 0; i < num; ++i)
    {
        auto player = team->getPlayer(i);
        auto& pos = player->getPosition();
        v.push_back(pos.x);
        v.push_back(pos.y);
        auto& vec = player->getMovingVector();
        v.push_back(vec.x);
        v.push_back(vec.y);
    }
    
    m_proxy->sendTeamPosition(v, team->isAttacking() ? team->getHilightPlayerId() : -1, (unsigned int)team->getSide());
}



void CFBMatch::teamPositionAck(int side, const vector<float>& p, int ballPlayerId, unsigned int timeStamp)
{
    auto team = m_teams[side];
    
    int size = team->getPlayerNumber();
    
    if (timeStamp == 0)     // timeStamp为0表示暂停时候的强制同步
    {
        for (int i = 0; i < size; ++i)
        {
            Point pos(p[i * 4], p[i * 4 + 1]);
            Point vec(p[i * 4 + 2], p[i * 4 + 3]);
            auto player = team->getPlayer(i);
            
            player->setPosition(pos);
            player->setMovingVector(vec);
        }
    }
    else
    {
        float dt = m_proxy->getDeltaTime(timeStamp);
        
        for (int i = 0; i < size; ++i)
        {
            Point pos(p[i * 4], p[i * 4 + 1]);
            Point vec(p[i * 4 + 2], p[i * 4 + 3]);
            auto player = team->getPlayer(i);

            player->moveFromTo(pos, vec, dt, m_SYNC_TIME);
        }
        
        CC_ASSERT(team == m_teamsInMatch[(int)SIDE::OPP]);
        m_syncTime[(int)SIDE::OPP] = m_SYNC_TIME;
    }
}



void CFBMatch::startMatchAck(unsigned int st)
{
    log("diff: %u", st - m_proxy->getTime());
    m_matchStep = FBDefs::MATCH_STEP::COUNT_DOWN;
    
    m_startTime = st;
    
    m_syncTime[(int)SIDE::SELF] = m_SYNC_TIME;
}



void CFBMatch::matchInfoAck(FBDefs::SIDE mySide, FBDefs::SIDE kickOffSide, int kickOffPlayer)
{

    log("side: %d, kick: %d", (int)mySide, (int)kickOffSide);

    setControlSide(mySide);
    m_teamsInMatch[(int)SIDE::SELF]->onStartMatch(false);
    m_teamsInMatch[(int)SIDE::OPP]->onStartMatch(true);
    
    m_teams[(int)kickOffSide]->kickOff(kickOffPlayer);
    
    if (m_teamsInMatch[(int)SIDE::SELF]->isAttacking())
    {
        m_matchUI->showAttackMenu(true);
    }
}



void CFBMatch::endMatchAck()
{
    m_matchUI->onGameEnd();
}



void CFBMatch::triggerMenuAck(FBDefs::MENU_TYPE menuType, vector<int>& attackPlayerNumbers, vector<int>& defendPlayerNumbers)
{
    int side = getControlSideTeam()->isAttacking() ? 0 : 1;
    m_matchUI->onMenu(menuType, attackPlayerNumbers, defendPlayerNumbers, side);
    m_playerInstructions.clear();
    
    m_attackPlayerNumbers = attackPlayerNumbers;
    m_defendPlayerNumbers = defendPlayerNumbers;
    m_targetPlayerId = -1;
}


void CFBMatch::instructionAck(unsigned int countDown)
{
    if (countDown  > 0)
    {
        // TODO: 收到countDown后更新客户端显示的倒计时
    }
    else
    {
        m_playerInstructions.clear();
        m_matchUI->waitInstruction();
    }
}



void CFBMatch::instructionResultAck()
{
    m_matchStep = FBDefs::MATCH_STEP::PLAY_ANIM;
    m_playAnimIndex = 0;
    
    playAnimInInstructionsResult();
}



CFBInstructionResult& CFBMatch::getInstructionResult()
{
    return m_instructionResult;
}


void CFBMatch::addPlayer(FBDefs::SIDE side, const CFBPlayerInitInfo& info)
{
    m_teams[(int)side]->addPlayer(info);
}