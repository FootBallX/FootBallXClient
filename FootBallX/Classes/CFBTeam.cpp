//
//  CFBTeam.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBTeam.h"
#include "CFBFormation.h"
#include "CFBPlayer.h"
#include "CFBMatch.h"

CFBTeam::CFBTeam()
: m_side(FBDefs::SIDE::NONE)
{
    
}



bool CFBTeam::init(const vector<string>& cardPlayers)
{
    do
    {
        // player 对象应该从哪里来？服务器？这里暂时创建一下。
        for (int i = 0; i < 11; ++i)
        {
            auto player = new CFBPlayer(cardPlayers[i]);
            player->m_ownerTeam = this;
            m_teamMembers.push_back(player);
        }
        m_formation = new CFBFormation442();
        BREAK_IF_FAILED(m_formation->init(this));
        
        m_state = FBDefs::TEAM_STATE::DEFENDING;
        return true;
    } while (false);
    
    return false;
}



void CFBTeam::update(float dt)
{
    if (this->isAttacking())
    {
        updateFieldStatusOnAttack();
    }
    else if (this->isDefending())
    {
        updateFieldStatusOnDefend();
    }
    
    m_formation->update(dt);
    
    function<bool(float, float)> comp =
        getSide() == FBDefs::SIDE::LEFT ?
        [](float a, float b){return a < b;} : [](float a, float b){return a > b;};
    
    m_lastPosOfPlayer = 0;
    for (auto x : m_teamMembers)
    {
        if (x->m_isOnDuty && x->m_isGoalKeeper == false)
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



bool CFBTeam::onStartMatch()
{
    do
    {
        CC_ASSERT(m_teamMembers.size() >= m_formation->getPlayerNumber());
        
        for (int i = 0; i < m_formation->getPlayerNumber(); ++i)
        {
            m_formation->addPlayer(m_teamMembers[i], i);
        }
        
        BREAK_IF_FAILED(m_formation->onStartMatch());
        
        return true;
    } while (false);
    
    return false;
}



void CFBTeam::kickOff()
{
    auto player = m_formation->getKickOffPlayer();
    player->gainBall();
    m_state = FBDefs::TEAM_STATE::ATTACKING;
}



CFBPlayer* CFBTeam::getHilightPlayer()
{
    if (m_hilightPlayerId >= 0 && m_hilightPlayerId < m_formation->getPlayerNumber())
    {
        return m_formation->getPlayer(m_hilightPlayerId);
    }
    
    return nullptr;
}



bool CFBTeam::changeFormation(FBDefs::FORMATION formationId)
{
    if (formationId == m_formation->getFormationId()) return true;
    
    switch (formationId)
    {
        case FBDefs::FORMATION::F_4_4_2:
            do
            {
                CC_SAFE_DELETE(m_formation);
                m_formation = new CFBFormation442();
                m_formation->init(this);
                return true;
            } while (false);
            return false;
        case FBDefs::FORMATION::F_3_5_2:
            do
            {
                CC_SAFE_DELETE(m_formation);
                m_formation = new CFBFormation352();
                m_formation->init(this);
                return true;
            } while (false);
            return false;
        default:
            break;
    }
    
    return false;
}



void CFBTeam::updateFieldStatusOnAttack()
{
    auto pitch = FBMATCH->getPitch();
    auto pp = getHilightPlayer();
    CC_ASSERT(pp && pp->m_isBallController);
    
    float size = pitch->transformPersentageX(0.4);
    size *= size;
    
    vector<int> gridsAroundPlayer;
    
    for (auto player : m_teamMembers)
    {
        auto ai = m_formation->getPlayerAIById(player->m_positionInFormation);
        ai->setPassBallScore(INT_MIN);
        
        if (player->m_isOnDuty)
        {
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
                if (dist > size)
                {
                    ai->increasePassBallScore((dist - size) * (-1));
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
    }
    
    auto side = getSide();
    auto otherSide = pitch->getOtherSide(side);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto teamMembers = otherTeam->getTeamMembers();
    for (auto player : teamMembers)
    {
        if (player->m_isOnDuty)
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
        if (t->m_isOnDuty && FBDefs::isPointOnTheWay(player->getPosition(), gp, t->getPosition()))
        {
            return false;
        }
    }
    
    teamMember = getTeamMembers();
    for (auto t : teamMember)
    {
        if (t != player && t->m_isOnDuty && FBDefs::isPointOnTheWay(player->getPosition(), gp, t->getPosition()))
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
        if (t->m_isOnDuty && FBDefs::isPointOnTheWay(player->getPosition(), ballPos, t->getPosition()))
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
    
    float size = pitch->transformPersentageX(0.4);
    size *= size;
    
    for (auto t : teamMember)
    {
        if (t->m_isOnDuty)
        {
            float dist = t->getPosition().getDistanceSq(player->getPosition());
            if (dist < size)
            {
                num++;
            }
        }
    }
    
    return num;
}



void CFBTeam::switchHilightPlayer()
{
    CC_ASSERT(isDefending());
    m_hilightPlayerId++;
    if (m_hilightPlayerId >= m_formation->getPlayerNumber())
    {
        m_hilightPlayerId = 1;
    }
}

