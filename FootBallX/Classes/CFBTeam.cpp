//
//  CFBTeam.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBTeam.h"
#include "CFBPlayer.h"
#include "CFBMatch.h"
#include "CFBPlayerAI.h"

CFBTeam::CFBTeam(FBDefs::SIDE side)
: m_side(side)
{
    m_state = FBDefs::TEAM_STATE::DEFENDING;
}



bool CFBTeam::init()
{
    do
    {
        m_state = FBDefs::TEAM_STATE::DEFENDING;
        return true;
    } while (false);
    
    return false;
}


int CFBTeam::getPlayerNumber()
{
    return (int)m_teamMembers.size();
}



void CFBTeam::update(float dt)
{
    auto comp =
        getSide() == FBDefs::SIDE::LEFT ?
        [](float a, float b){return a < b;} : [](float a, float b){return a > b;};
    
    m_lastPosOfPlayer = 0;
    for (auto x : m_teamMembers)
    {
        x->getBrain()->update(dt);
        if (x->m_isGoalKeeper == false)
        {
            auto& pos = x->getPosition();
            if (m_lastPosOfPlayer == 0)
            {
                m_lastPosOfPlayer = pos.x;
            }
            if (comp(pos.x, m_lastPosOfPlayer))
            {
                m_lastPosOfPlayer = pos.x;
            }
        }
    }
}



void CFBTeam::think()
{
    if (this->isAttacking())
    {
        updateFieldStatusOnAttack();
    }
    else if (this->isDefending())
    {
        updateFieldStatusOnDefend();
    }
    
    for (auto player : m_teamMembers)
    {
        player->getBrain()->think();
    }
}



void CFBTeam::addPlayer(const CFBPlayerInitInfo& info)
{
    CFBPlayer* player = new CFBPlayer(this, info.card);
    player->setPosition(info.position);
    
    switch (info.aiClass)
    {
        case 0:
            player->createBrain(FBDefs::AI_CLASS::GOAL_KEEPER, info.homePosition, FBDefs::GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            player->createBrain(FBDefs::AI_CLASS::BACK, info.homePosition, FBDefs::BACK_ORBIT_RATE);
            break;
        case 2:
            player->createBrain(FBDefs::AI_CLASS::HALF_BACK, info.homePosition, FBDefs::HALF_BACK_ORBIT_RATE);
            break;
        case 3:
            player->createBrain(FBDefs::AI_CLASS::FORWARD, info.homePosition, FBDefs::FORWARD_ORBIT_RATE);
            break;
        default:
            CC_ASSERT(false);
            break;
    }

    m_teamMembers.push_back(player);
    
    player->m_positionInFormation = (int)m_teamMembers.size() - 1;
}



bool CFBTeam::onStartMatch(bool networkControl)
{
    do
    {
        int num = (int)m_teamMembers.size();
        for (auto player : m_teamMembers)
        {
            player->getBrain()->setNetworkControl(networkControl);
        }
        setHilightPlayerId(num - 1);
        return true;
    } while (false);
    
    return false;
}



void CFBTeam::kickOff(int playerNumber)
{
    auto player = getPlayer(playerNumber);
    player->gainBall();
    m_state = FBDefs::TEAM_STATE::ATTACKING;
}



CFBPlayer* CFBTeam::getHilightPlayer()
{
    return getPlayer(m_hilightPlayerId);
}


CFBPlayer* CFBTeam::getPlayer(int idx)
{
    if (idx >= 0 && idx < m_teamMembers.size())
    {
        return m_teamMembers[idx];
    }
    
    CC_ASSERT(false);
    return nullptr;
}



void CFBTeam::updateFieldStatusOnAttack()
{
    auto pitch = FBMATCH->getPitch();
    auto pp = getHilightPlayer();
    CC_ASSERT(pp && pp->m_isBallController);
    
    float sizeSq = FBDefs::PASS_BALL_REDUCTION * FBDefs::PASS_BALL_REDUCTION;
    
    vector<int> gridsAroundPlayer;
    
    for (auto player : m_teamMembers)
    {
        auto ai = player->getBrain();
        ai->setPassBallScore(INT_MIN);
        
        auto& pos = player->getPosition();
        
        if (!player->m_isGoalKeeper && pp != player)
        {
            ai->setPassBallScore(0);
            
            // can shoot directly?
            if (canShootDirectly(player))
            {
                ai->increasePassBallScore(50);
            }
            
            int num = getNumberOfDefenderBetweenPlayerAndBall(player);
            ai->increasePassBallScore(-20 * num);
            
            num = getNumberOfDefenderAroundPlayer(player);
            ai->increasePassBallScore(-10 * num);
            
            float dist = pp->getPosition().getDistanceSq(pos);
            if (dist > sizeSq)
            {
                ai->increasePassBallScore((dist - sizeSq) * (-1));
            }
        }
        
        if (pitch->getGridsAroundPosition(pos, gridsAroundPlayer))
        {
            for (auto gid : gridsAroundPlayer)
            {
                pitch->increaseGridDefenceScore(gid, -10);
            }
        }
        
    }
    
    auto side = getSide();
    auto otherSide = pitch->getOtherSide(side);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto teamMembers = otherTeam->getTeamMembers();
    for (auto player : teamMembers)
    {
        if (pitch->getGridsAroundPosition(player->getPosition(), gridsAroundPlayer))
        {
            for (auto gid : gridsAroundPlayer)
            {
                pitch->increaseGridDefenceScore(gid, -20);
            }
        }
    }
}



void CFBTeam::updateFieldStatusOnDefend()
{
    
}



bool CFBTeam::canShootDirectly(CFBPlayer* player)
{
    auto side = getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(side);
    auto gp = pitch->getGoalPos(otherSide);
    
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto teamMember = otherTeam->getTeamMembers();
    for (auto t : teamMember)
    {
        if (FBDefs::isPointOnTheWay(player->getPosition(), gp, t->getPosition()))
        {
            return false;
        }
    }
    
    teamMember = getTeamMembers();
    for (auto t : teamMember)
    {
        if (t != player && FBDefs::isPointOnTheWay(player->getPosition(), gp, t->getPosition()))
        {
            return false;
        }
    }
    return true;
}



int CFBTeam::getNumberOfDefenderBetweenPlayerAndBall(CFBPlayer* player)
{
    int num = 0;
    auto side = getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(side);
    auto& ballPos = FBMATCH->getBallPosition();
    
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto teamMember = otherTeam->getTeamMembers();
    for (auto t : teamMember)
    {
        if (FBDefs::isPointOnTheWay(player->getPosition(), ballPos, t->getPosition()))
        {
            num++;
        }
    }
    
    return num;
}



int CFBTeam::getNumberOfDefenderAroundPlayer(CFBPlayer* player)
{
    int num = 0;
    auto side = getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(side);
    
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto teamMember = otherTeam->getTeamMembers();
    
    float sizeSq = FBDefs::DEFENDER_PLAYER_RADIUS * FBDefs::DEFENDER_PLAYER_RADIUS;
    
    for (auto t : teamMember)
    {
        float dist = t->getPosition().getDistanceSq(player->getPosition());
        if (dist < sizeSq)
        {
            num++;
        }
    }
    
    return num;
}



void CFBTeam::switchHilightPlayer()
{
    CC_ASSERT(isDefending());
    m_hilightPlayerId++;
    if (m_hilightPlayerId >= getPlayerNumber())
    {
        m_hilightPlayerId = 1;
    }
}



CFBPlayer* CFBTeam::getPassBallTarget()
{
    int max = INT_MIN;
    CFBPlayer* player = nullptr;
    
    for (auto p : m_teamMembers)
    {
        int score = p->getBrain()->getPassBallScore();
        if (score > max)
        {
            max = score;
            player = p;
        }
    }
    
    return player;
}



FBDefs::SIDE CFBTeam::getSide() const
{
    return m_side;
}
