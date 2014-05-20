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
#include "CRandomManager.h"

bool CFBPlayerAI::init(CFBTeam* team, CFBPlayer* player, const Point& homePos, float orbit)
{
    do
    {
        BREAK_IF(player == nullptr);
        BREAK_IF(team == nullptr);
        m_team = team;
        m_player = player;
        
        m_homePosition = homePos;
        m_origHomePosition = m_homePosition;
        
        m_defendOrbitRadius = orbit;
        m_defendOrbitRadiusSq = m_defendOrbitRadius * m_defendOrbitRadius;
        m_defendOrbitRadiusx2Sq = (2 * m_defendOrbitRadius) * (2 * m_defendOrbitRadius);
        
        m_state = FBDefs::AI_STATE::NONE;
        
        return true;
    } while (false);
    
    return false;
}


void CFBPlayerAI::setNetworkControl(bool networkControl)
{
    if (networkControl)
    {
        m_state = FBDefs::AI_STATE::NETWORK;
    }
    else
    {
        m_state = FBDefs::AI_STATE::NONE;
    }
}


void CFBPlayerAI::think()
{
    if (m_state == FBDefs::AI_STATE::NETWORK) return;
    
    if (m_team->getHilightPlayer() != m_player)
    {
        if (m_team->isAttacking())
        {
            thinkOnAttacking();
        }
        else if (m_team->isDefending())
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
        auto homePos = pitch->transformBySide(m_homePosition, m_team->getSide());
        
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
        auto homePos = pitch->transformBySide(m_homePosition, m_team->getSide());
        
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
    auto homePos = pitch->transformBySide(m_homePosition, m_team->getSide());

    m_player->moveTo(homePos);
}



void CFBPlayerAI::chaseBall(float dt)
{
    CC_ASSERT(m_player);
    auto pitch = FBMATCH->getPitch();
    auto homePos = pitch->transformBySide(m_homePosition, m_team->getSide());
    
    if (m_team->isDefending())
    {
        auto ballPos = FBMATCH->getBallPosition();
        
        auto ls = (homePos - m_player->getPosition()).getLengthSq();
        if (ls >= m_defendOrbitRadiusx2Sq)
        {
            m_state = FBDefs::AI_STATE::BACKHOME;
            
            auto actp = m_team->getActivePlayer();
            auto assp = m_team->getAssistantPlayer();
            
            if (actp == m_player->m_positionInFormation)
            {
                m_team->setActivePlayer(-1);
                
                if (assp > -1)
                {
                    auto playerAI = m_team->getPlayer(assp)->getBrain();
                    playerAI->m_state = FBDefs::AI_STATE::BACKHOME;
                    
                    m_team->setAssistantPlayer(-1);
                }
            }
            else if (assp == m_player->m_positionInFormation)
            {
                m_team->setAssistantPlayer(-1);
            }
        }
        else
        {
            auto actp = m_team->getActivePlayer();
            if (actp == -1)
            {
                m_team->setActivePlayer(m_player->m_positionInFormation);
            }
            else if (actp == m_player->m_positionInFormation)
            {
                m_player->moveTo(ballPos);
            }
            else
            {
                auto assp = m_team->getAssistantPlayer();
                if (assp == -1)
                {
                    m_team->setAssistantPlayer(m_player->m_positionInFormation);
                }
                else if (assp == m_player->m_positionInFormation)
                {
                    m_player->moveTo(pitch->getBestAssistantDeffendingPosition(ballPos, m_team->getSide()));
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
    const Point& ballPos = FBMATCH->getBallPosition();
    auto pitch = FBMATCH->getPitch();
    auto homePos = pitch->transformBySide(m_homePosition, m_team->getSide());

    if (m_state != FBDefs::AI_STATE::CHASE)
    {
        if ((homePos - ballPos).getLengthSq() < m_defendOrbitRadiusSq)
        {
            m_state = FBDefs::AI_STATE::CHASE;
        }
    }
    
    if (m_state == FBDefs::AI_STATE::CHASE)
    {
        auto actp = m_team->getActivePlayer();
        auto assp = m_team->getAssistantPlayer();
        
        if ((homePos - m_player->getPosition()).getLengthSq() >= m_defendOrbitRadiusx2Sq)
        {
            m_state = FBDefs::AI_STATE::NONE;
            
            if (actp == m_player->m_positionInFormation)
            {
                m_team->setActivePlayer(-1);
                
                if (assp > -1)
                {
                    auto playerAI = m_team->getPlayer(assp)->getBrain();
                    playerAI->m_state = FBDefs::AI_STATE::NONE;
                    
                    m_team->setAssistantPlayer(-1);
                }
            }
            else if (assp == m_player->m_positionInFormation)
            {
                m_team->setAssistantPlayer(-1);
            }
        }
        else
        {
            if (actp == -1 || actp == m_player->m_positionInFormation)
            {
                m_team->setActivePlayer(m_player->m_positionInFormation);
                m_player->moveTo(ballPos);
            }
            else if (assp == -1 || assp == m_player->m_positionInFormation)
            {
                m_team->setAssistantPlayer(m_player->m_positionInFormation);
                m_player->moveTo(pitch->getBestAssistantDeffendingPosition(ballPos, m_team->getSide()));
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
            if (m_team->getSide() == FBDefs::SIDE::LEFT)
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
    FBDefs::SIDE side = m_team->getSide();
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
    
    if (rNum < 50 && m_team->canShootDirectly(this->m_player))
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
            m_team->updateFieldStatusOnAttack();
            
            auto target = m_team->getPassBallTarget();
            if (target)
            {
                FBMATCH->tryPassBall(m_player, target);
            }
        }
    }
}



void CFBPlayerAI::thinkDribbleBall()
{
    if (m_controlState != FBDefs::AI_STATE_CONTROL::NONE) return;
    
    auto side = m_team->getSide();
    auto pitch = FBMATCH->getPitch();
    auto otherSide = pitch->getOtherSide(side);
    auto otherTeam = FBMATCH->getTeam(otherSide);
    auto& otherTeamMembers = otherTeam->getTeamMembers();
    auto& teamMembers = m_team->getTeamMembers();
    
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
        if (FBDefs::isPointOnTheWay(p1, goalDirPos, player->getPosition()))
        {
            ok = false;
            break;
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
        if (FBDefs::isPointOnTheWay(p1, aheadPos, player->getPosition()))
        {
            ok = false;
            break;
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
    
    m_team->updateFieldStatusOnAttack();
    auto target = m_team->getPassBallTarget();
    
    if (target)
    {
        FBMATCH->tryPassBall(m_player, target);
    }
}


