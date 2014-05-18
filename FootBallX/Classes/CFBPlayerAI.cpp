//
//  CFBPlayerAI.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-19.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPlayerAI.h"
#include "CFBPlayer.h"
#include "CFBMatch.h"
#include "CFBFormation.h"
#include "CRandomManager.h"

bool CFBPlayerAI::init(CFBFormation* formation, CFBPlayer* player, const Point& pos, const Point& homePos, float radius, bool networkControl)
{
    do
    {
        BREAK_IF(player == nullptr);
        BREAK_IF(formation == nullptr);
        m_formation = formation;
        m_player = player;
        
        m_player->setPosition(pos);
        
        m_initPosition = pos;
        m_homePosition = homePos;
        m_origHomePosition = m_homePosition;
        
        m_defendOrbitRadius = radius;
        m_defendOrbitRadiusSq = m_defendOrbitRadius * m_defendOrbitRadius;
        m_defendOrbitRadiusx2Sq = (2 * m_defendOrbitRadius) * (2 * m_defendOrbitRadius);
        
        m_player->m_isOnDuty = true;
        
        if (networkControl)
        {
            m_state = FBDefs::AI_STATE::NETWORK;
        }
        else
        {
            m_state = FBDefs::AI_STATE::NONE;
        }
        
        return true;
    } while (false);
    
    return false;
}



void CFBPlayerAI::think()
{
    if (m_state == FBDefs::AI_STATE::NETWORK) return;
    
    auto team = m_formation->getTeam();
    if (team->getHilightPlayer() != m_player)
    {
        if (team->isAttacking())
        {
            thinkOnAttacking();
        }
        else if (team->isDefending())
        {
            thinkOnDefending();
        }
    }
    else
    {
        m_state = FBDefs::AI_STATE::USER_CONTROL;
    }
}



void CFBPlayerAI::thinkOnAttacking()
{
    considerSupport();
    if (m_state == FBDefs::AI_STATE::NONE)
    {
        updateHomePosition();
        
        auto pitch = FBMATCH->getPitch();
        auto team = m_formation->getTeam();
        auto homePos = pitch->transformBySide(m_homePosition, team->getSide());
        
        m_player->moveTo(homePos);
    }
}



void CFBPlayerAI::thinkOnDefending()
{
    considerChase();
    if (m_state == FBDefs::AI_STATE::NONE)
    {
        updateHomePosition();
        
        auto pitch = FBMATCH->getPitch();
        auto team = m_formation->getTeam();
        auto homePos = pitch->transformBySide(m_homePosition, team->getSide());
        
        m_player->moveTo(homePos);
    }
}



void CFBPlayerAI::update(float dt)
{
    m_player->update(dt);
}



void CFBPlayerAI::considerSupport()
{
}



//void CFBPlayerAI::initPlayerStates(const Point& pt, bool networkControl)
//{
//    m_player->setPosition(pt);
//    m_player->m_isOnDuty = true;
//    
//    if (networkControl)
//    {
//        m_state = FBDefs::AI_STATE::NETWORK;
//    }
//    else
//    {
//        m_state = FBDefs::AI_STATE::NONE;
//    }
//}



void CFBPlayerAI::returnToHome(float dt)
{
    CC_ASSERT(m_player);
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    auto homePos = pitch->transformBySide(m_homePosition, team->getSide());

    m_player->moveTo(homePos);
}



void CFBPlayerAI::chaseBall(float dt)
{
    CC_ASSERT(m_player);
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    auto homePos = pitch->transformBySide(m_homePosition, team->getSide());
    
    if (m_formation->getTeam()->isDefending())
    {
        auto ballPos = FBMATCH->getBallPosition();
        
        auto ls = (homePos - m_player->getPosition()).getLengthSq();
        if (ls >= m_defendOrbitRadiusx2Sq)
        {
            m_state = FBDefs::AI_STATE::BACKHOME;
            
            auto actp = team->getActivePlayer();
            auto assp = team->getAssistantPlayer();
            
            if (actp == m_player->m_positionInFormation)
            {
                team->setActivePlayer(-1);
                
                if (assp > -1)
                {
                    auto playerAI = m_formation->getPlayerAIById(assp);
                    playerAI->m_state = FBDefs::AI_STATE::BACKHOME;
                    
                    team->setAssistantPlayer(-1);
                }
            }
            else if (assp == m_player->m_positionInFormation)
            {
                team->setAssistantPlayer(-1);
            }
        }
        else
        {
            auto actp = team->getActivePlayer();
            if (actp == -1)
            {
                team->setActivePlayer(m_player->m_positionInFormation);
            }
            else if (actp == m_player->m_positionInFormation)
            {
                m_player->moveTo(ballPos);
            }
            else
            {
                auto assp = team->getAssistantPlayer();
                if (assp == -1)
                {
                    team->setAssistantPlayer(m_player->m_positionInFormation);
                }
                else if (assp == m_player->m_positionInFormation)
                {
                    m_player->moveTo(pitch->getBestAssistantDeffendingPosition(ballPos, team->getSide()));
                }
                else
                {
                    this->m_state = FBDefs::AI_STATE::BACKHOME;
                }
            }
        }
    }
}



void CFBPlayerAI::considerChase()
{
    auto team = m_formation->getTeam();
    const Point& ballPos = FBMATCH->getBallPosition();
    auto pitch = FBMATCH->getPitch();
    auto homePos = pitch->transformBySide(m_homePosition, team->getSide());

    if (m_state != FBDefs::AI_STATE::CHASE)
    {
        if ((homePos - ballPos).getLengthSq() < m_defendOrbitRadiusSq)
        {
            m_state = FBDefs::AI_STATE::CHASE;
        }
    }
    
    if (m_state == FBDefs::AI_STATE::CHASE)
    {
        auto actp = team->getActivePlayer();
        auto assp = team->getAssistantPlayer();
        
        if ((homePos - m_player->getPosition()).getLengthSq() >= m_defendOrbitRadiusx2Sq)
        {
            m_state = FBDefs::AI_STATE::NONE;
            
            if (actp == m_player->m_positionInFormation)
            {
                team->setActivePlayer(-1);
                
                if (assp > -1)
                {
                    auto playerAI = m_formation->getPlayerAIById(assp);
                    playerAI->m_state = FBDefs::AI_STATE::NONE;
                    
                    team->setAssistantPlayer(-1);
                }
            }
            else if (assp == m_player->m_positionInFormation)
            {
                team->setAssistantPlayer(-1);
            }
        }
        else
        {
            if (actp == -1 || actp == m_player->m_positionInFormation)
            {
                team->setActivePlayer(m_player->m_positionInFormation);
                m_player->moveTo(ballPos);
            }
            else if (assp == -1 || assp == m_player->m_positionInFormation)
            {
                team->setAssistantPlayer(m_player->m_positionInFormation);
                m_player->moveTo(pitch->getBestAssistantDeffendingPosition(ballPos, team->getSide()));
            }
        }
    }
}



void CFBPlayerAI::updateWait(float dt)
{
    if (this->m_state == FBDefs::AI_STATE::WAIT)
    {
        m_waitTime -= dt;
        if (FLT_LE(m_waitTime, 0.f))
        {
            // debug
            if (m_formation->getTeam()->getSide() == FBDefs::SIDE::LEFT)
            {
                log("1");
            }
            // end
            this->m_state = FBDefs::AI_STATE::NONE;
        }
    }
}



void CFBPlayerAI::startWait(float t)
{
    m_waitTime = t;
    m_state = FBDefs::AI_STATE::WAIT;
}



void CFBPlayerAI::updateAIControlBall(float dt)
{
}



void CFBPlayerAI::PreventOffside(float& x)
{
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    FBDefs::SIDE side = team->getSide();
    FBDefs::SIDE otherSide = pitch->getOtherSide(side);
    
    float offsideLine = FBMATCH->getTeam(otherSide)->getLastPosOfPlayer();
    offsideLine = pitch->getPitchWidth() - pitch->transformBySide(offsideLine, otherSide);
    
    if (x > offsideLine)
    {
        x = offsideLine;
    }
}



void CFBPlayerAI::increasePassBallScore(int inc)
{
    m_passBallScore += inc;
}



void CFBPlayerAI::thinkControlBall()
{
    int rNum = RANDOM_MGR->getRand() % 100;
    auto team = m_formation->getTeam();
    
    if (rNum < 50 && team->canShootDirectly(this->m_player))
    {
        // shoot
    }
    else
    {
        rNum = RANDOM_MGR->getRand() % 100;
        if (rNum < 0)
        {
            // dribber
        }
        else
        {
            // pass
            team->updateFieldStatusOnAttack();
            
            auto target = m_formation->getPassBallTarget();
            if (target)
            {
                FBMATCH->tryPassBall(m_player, target->getPlayer());
            }
        }
    }
}



void CFBPlayerAI::thinkDribbleBall()
{
    if (m_controlState != FBDefs::AI_STATE_CONTROL::NONE) return;
    
    auto team = m_formation->getTeam();
    auto side = team->getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(side);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto& otherTeamMembers = otherTeam->getTeamMembers();
    auto& teamMembers = team->getTeamMembers();
    
    auto& p1 = m_player->getPosition();
    Point aheadPos = p1;
    
    auto checkLength = FBDefs::DRIBBLE_CHECK_DIST;
    
    if (side == FBDefs::SIDE::LEFT)
    {
        aheadPos.x += checkLength;
    }
    else
    {
        aheadPos.x -= checkLength;
    }
    
    Point goalDirPos = p1 + (pitch->getGoalPos(otherSide) - p1).normalize() * checkLength;
    
    
    bool ok = true;

    for (auto player : otherTeamMembers)
    {
        if (player->m_isOnDuty)
        {
            if (FBDefs::isPointOnTheWay(p1, goalDirPos, player->getPosition()))
            {
                ok = false;
                break;
            }
        }
    }
    
    if (ok)
    {
        m_controlState = FBDefs::AI_STATE_CONTROL::DRIBBLE;
        m_moveToTarget = goalDirPos;
        return;
    }
    
    for (auto player : otherTeamMembers)
    {
        if (player->m_isOnDuty)
        {
            if (FBDefs::isPointOnTheWay(p1, aheadPos, player->getPosition()))
            {
                ok = false;
                break;
            }
        }
    }
    
    if (ok)
    {
        m_controlState = FBDefs::AI_STATE_CONTROL::DRIBBLE;
        m_moveToTarget = aheadPos;
        return;
    }
}



void CFBPlayerAI::thinkPassBall()
{
    if (m_controlState != FBDefs::AI_STATE_CONTROL::NONE) return;
    
    auto team = m_formation->getTeam();
    
    team->updateFieldStatusOnAttack();
    auto target = m_formation->getPassBallTarget();
    
    if (target)
    {
        FBMATCH->tryPassBall(m_player, target->getPlayer());
    }
}


