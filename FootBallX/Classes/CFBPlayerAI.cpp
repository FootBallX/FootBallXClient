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

bool CFBPlayerAI::init(CFBFormation* formation, CFBPlayer* player, const Point& pos, const Point& homePos, float radius)
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
        return true;
    } while (false);
    
    return false;
}



void CFBPlayerAI::think()
{
    auto team = m_formation->getTeam();
    if (!FBMATCH->checkControlSide(team->getSide()) ||
        team->getHilightPlayer() != m_player)
    {
        if (m_state == FBDefs::AI_STATE::NONE)
        {
            m_state = FBDefs::AI_STATE::BACKHOME;
        }
        
        if (this->m_player->m_isBallController)
        {
            m_state = FBDefs::AI_STATE::CONTROL;
            thinkControlBall();
        }
        else
        {
            if (m_state == FBDefs::AI_STATE::CONTROL)   // TODO: 不好，应该在某个think函数中得出明确指令，这里要切什么状态。
            {
                m_state = FBDefs::AI_STATE::BACKHOME;
            }
            thinkHomePosition();
            thinkDefending();
        }
        
        updatePlayerStates();       // TODO: 这个函数功能和think重复，考虑以后合并。
    }
    else
    {
        m_state = FBDefs::AI_STATE::NONE;
    }
}


void CFBPlayerAI::thinkNoBall()
{
}



void CFBPlayerAI::thinkNoBallOnAttacking()
{
}



void CFBPlayerAI::thinkNoBallOnDefending()
{
}


void CFBPlayerAI::update(float dt)
{
    m_changeStateCD -= dt;
    m_player->update(dt);
}



void CFBPlayerAI::updatePlayerStates()
{
}



void CFBPlayerAI::initPlayerStates()
{
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    m_player->setPosition(pitch->transformBySide(m_initPosition, team->getSide()));
    m_player->m_isOnDuty = true;
    m_state = FBDefs::AI_STATE::BACKHOME;
}



void CFBPlayerAI::returnToHome(float dt)
{
    CC_ASSERT(m_player);
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    auto homePos = pitch->transformBySide(m_homePosition, team->getSide());
    if (!isOnPosition(homePos))
    {
        moveTo(homePos, dt);
    }
}



void CFBPlayerAI::moveTo(const Point& pos, float dt)
{
    auto position = m_player->getPosition();
    auto v = (pos - position).normalize();
    position += v * m_player->getSpeed() * dt;
    m_player->setPosition(position);
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
        if (ls >= m_defendOrbitRadiusx2Sq && isNotInStateCD())
        {
            applyStateCD();
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
                moveTo(ballPos, dt);
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
                    moveTo(pitch->getBestAssistantDeffendingPosition(ballPos, team->getSide()), dt);
                }
                else
                {
                    this->m_state = FBDefs::AI_STATE::BACKHOME;
                }
            }
        }
    }
}



bool CFBPlayerAI::isOnPosition(const Point& pos)
{
    auto pt = m_player->getPosition() - pos;
    if (pt.getLengthSq() < 10)
    {
        return true;
    }
    
    return false;
}



void CFBPlayerAI::thinkDefending()
{
    if (m_formation->getTeam()->isDefending() && m_state != FBDefs::AI_STATE::CHASE)
    {
        const Point& ballPos = FBMATCH->getBallPosition();
        auto pitch = FBMATCH->getPitch();
        auto team = m_formation->getTeam();
        auto homePos = pitch->transformBySide(m_homePosition, team->getSide());

        if ((homePos - ballPos).getLengthSq() < m_defendOrbitRadiusSq && isNotInStateCD())
        {
            applyStateCD();
            m_state = FBDefs::AI_STATE::CHASE;
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


