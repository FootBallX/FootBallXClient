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
            if (m_lastPosOfPlayer == 0)
            {
                m_lastPosOfPlayer = x->m_curPosition.x;
            }
            if (comp(x->m_curPosition.x, m_lastPosOfPlayer))
            {
                m_lastPosOfPlayer = x->m_curPosition.x;
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



void CFBTeam::passBall(int passTo)
{   
    CC_ASSERT(passTo >=0 && passTo < m_teamMembers.size());
    auto pp = getPlayingPlayer();
    pp->loseBall();
    m_teamMembers[passTo]->gainBall();
}



CFBPlayer* CFBTeam::getPlayingPlayer()
{
    for (int i = 0; i < m_teamMembers.size(); ++i)
    {
        auto player = m_teamMembers[i];
        if (player->m_isOnDuty && player->m_isBallController)
        {
            return player;
        }
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
    auto pp = this->getPlayingPlayer();
    auto pitch = FBMATCH->getPitch();
    
    float size = pitch->transformPersentageX(0.4);
    size *= size;
    
    vector<int> gridsAroundPlayer;
    
    for (auto player : m_teamMembers)
    {
        auto ai = m_formation->getPlayerAIById(player->m_positionInFormation);
        ai->setPassBallScore(INT_MIN);
        
        if (player->m_isOnDuty)
        {
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
                
                float dist = pp->m_curPosition.getDistanceSq(player->m_curPosition);
                if (dist > size)
                {
                    ai->increasePassBallScore((dist - size) * (-1));
                }
            }
            
            if (pitch->getGridsAroundPosition(player->m_curPosition, gridsAroundPlayer))
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
            if (pitch->getGridsAroundPosition(player->m_curPosition, gridsAroundPlayer))
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
        if (t->m_isOnDuty && FBDefs::isPointOnTheWay(player->m_curPosition, gp, t->m_curPosition))
        {
            return false;
        }
    }
    
    teamMember = getTeamMembers();
    for (auto t : teamMember)
    {
        if (t != player && t->m_isOnDuty && FBDefs::isPointOnTheWay(player->m_curPosition, gp, t->m_curPosition))
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
    auto ball = FBMATCH->getBall();
    auto& ballPos = ball->getBallPos();
    
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto teamMember = otherTeam->getTeamMembers();
    for (auto t : teamMember)
    {
        if (t->m_isOnDuty && FBDefs::isPointOnTheWay(player->m_curPosition, ballPos, t->m_curPosition))
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
            float dist = t->m_curPosition.getDistanceSq(player->m_curPosition);
            if (dist < size)
            {
                num++;
            }
        }
    }
    
    return num;
}


