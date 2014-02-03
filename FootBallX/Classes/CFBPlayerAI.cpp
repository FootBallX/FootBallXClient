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

#pragma mark ----- AI Base

bool CFBPlayerAI::init(CFBFormation* formation, CFBPlayer* player, float posX, float posY, float radius)
{
    do
    {
        BREAK_IF(player == nullptr);
        BREAK_IF(formation == nullptr);
        m_formation = formation;
        m_player = player;
        Point pt(posX, posY);
        auto pitch = FBMATCH->getPitch();
        m_attackPosition = pitch->transformPersentage(pt, m_formation->getTeam()->getSide());
        pt.x *= 0.5f;
        m_defendPosition = pitch->transformPersentage(pt, m_formation->getTeam()->getSide());
        m_homePosition = m_defendPosition;
        
        m_defendOrbitRadius = pitch->transformPersentage(radius);
        m_defendOrbitRadiusSq = m_defendOrbitRadius * m_defendOrbitRadius;
        m_defendOrbitRadiusx2Sq = (2 * m_defendOrbitRadius) * (2 * m_defendOrbitRadius);
        return true;
    } while (false);
    
    return false;
}



void CFBPlayerAI::think()
{
    thinkHomePosition();
    thinkDefending();
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
    m_player->m_curPosition = m_defendPosition;
    m_player->m_isOnDuty = true;
    m_state = FBDefs::AI_STATE::BACKHOME;
}



void CFBPlayerAI::returnToPosition(float dt)
{
    CC_ASSERT(m_player);

    if (!isOnHomePosition())
    {
        auto v = (m_homePosition - m_player->m_curPosition).normalize();
        m_player->m_curPosition += v * m_player->getSpeed() * dt;
    }
}



void CFBPlayerAI::chaseBall(float dt)
{
    CC_ASSERT(m_player);
    if (m_formation->getTeam()->isDefending())
    {
        auto ballPos = FBMATCH->getBall()->getBallPos();
        
        auto ls = (m_homePosition - m_player->m_curPosition).getLengthSq();
        if (ls >= m_defendOrbitRadiusx2Sq && isNotInStateCD())
        {
            applyStateCD();
            m_state = FBDefs::AI_STATE::BACKHOME;
        }
        else
        {
            auto v = (ballPos - m_player->m_curPosition).normalize();
            m_player->m_curPosition += v * m_player->getSpeed() * dt;
        }
    }
}



bool CFBPlayerAI::isOnHomePosition()
{
    auto pt = m_player->m_curPosition - m_homePosition;
    if (pt.getLengthSq() < 10)
    {
        return true;
    }
    
    return false;
}



void CFBPlayerAI::thinkHomePosition()
{
    auto team = m_formation->getTeam();
    auto pitch = FBMATCH->getPitch();
    auto ball = FBMATCH->getBall();
    int pitchWidth = pitch->getPitchWidth();
    int halfPitchWidth = pitchWidth * 0.5f;
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto ballPos = ball->getBallPos();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = halfPitchHeight * yRate * .2;
    
    if (team->isAttacking())
    {
        if (FBDefs::SIDE::LEFT == team->getSide())
        {
            float diff = ballPos.x;
            if (diff > 0)
            {
                m_homePosition = m_defendPosition +  (m_attackPosition - m_defendPosition) * (diff / pitchWidth);
                m_homePosition.y += yOffset;
                return;
            }
        }
        else if (FBDefs::SIDE::RIGHT == team->getSide())
        {
            float diff = pitchWidth - ballPos.x;
            if (diff > 0)
            {
                m_homePosition = m_defendPosition + (m_attackPosition - m_defendPosition) * (diff / pitchWidth);
                m_homePosition.y += yOffset;
                return;
            }
        }

        m_homePosition = m_defendPosition;
        m_homePosition.y += yOffset;
    }
    else if (team->isDefending())
    {
        if (FBDefs::SIDE::LEFT == team->getSide())
        {
            float diff = ballPos.x - halfPitchWidth;
            if (diff > 0)
            {
                m_homePosition = m_defendPosition -  (m_defendPosition - m_attackPosition) * (diff / halfPitchWidth);
                m_homePosition.y += yOffset;
                return;
            }
        }
        else if (FBDefs::SIDE::RIGHT == team->getSide())
        {
            float diff = halfPitchWidth - ballPos.x;
            if (diff > 0)
            {
                m_homePosition = m_defendPosition - (m_defendPosition - m_attackPosition) * (diff / halfPitchWidth);
                m_homePosition.y += yOffset;
                return;
            }
        }

        m_homePosition = m_defendPosition;
        m_homePosition.y += yOffset;
    }
}



void CFBPlayerAI::thinkDefending()
{
    if (m_formation->getTeam()->isDefending() && m_state != FBDefs::AI_STATE::CHASE)
    {
        const Point& ballPos = FBMATCH->getBall()->getBallPos();
        const Point& homePos = m_homePosition;
        if ((homePos - ballPos).getLengthSq() < m_defendOrbitRadiusSq && isNotInStateCD())
        {
            applyStateCD();
            m_state = FBDefs::AI_STATE::CHASE;
        }
    }
}


#pragma mark ----- Goalkeeper AI

void CFBGoalkeeperAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    
    switch (m_state)
    {
        case FBDefs::AI_STATE::BACKHOME:
            returnToPosition(dt);
            break;
        case FBDefs::AI_STATE::WAIT:
            break;
        default:
            break;
    }
}



void CFBGoalkeeperAI::updatePlayerStates()
{
    CFBPlayerAI::updatePlayerStates();
}


void CFBGoalkeeperAI::initPlayerStates()
{
    CFBPlayerAI::initPlayerStates();
    
    m_player->m_isGoalKeeper = true;
}

#pragma mark ----- Back AI

void CFBBackAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    switch (m_state)
    {
        case FBDefs::AI_STATE::BACKHOME:
            returnToPosition(dt);
            break;
        case FBDefs::AI_STATE::WAIT:
            break;
        case FBDefs::AI_STATE::CHASE:
            chaseBall(dt);
            break;
        default:
            break;
    }
}



void CFBBackAI::updatePlayerStates()
{
    CFBPlayerAI::updatePlayerStates();
}



void CFBBackAI::initPlayerStates()
{
    CFBPlayerAI::initPlayerStates();
    
    m_player->m_isGoalKeeper = false;
}


#pragma mark ----- Half back AI

void CFBHalfBackAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    switch (m_state)
    {
        case FBDefs::AI_STATE::BACKHOME:
            returnToPosition(dt);
            break;
        case FBDefs::AI_STATE::WAIT:
            break;
        case FBDefs::AI_STATE::CHASE:
            chaseBall(dt);
            break;
        default:
            break;
    }
}



void CFBHalfBackAI::updatePlayerStates()
{
    CFBPlayerAI::updatePlayerStates();

}



void CFBHalfBackAI::initPlayerStates()
{
    CFBPlayerAI::initPlayerStates();
    
    m_player->m_isGoalKeeper = false;
}




#pragma mark ----- Forward AI

void CFBForwardAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    if (!this->m_player->m_isBallController)
    {
        switch (m_state)
        {
            case FBDefs::AI_STATE::BACKHOME:
                returnToPosition(dt);
                break;
            case FBDefs::AI_STATE::WAIT:
                break;
            case FBDefs::AI_STATE::CHASE:
                chaseBall(dt);
                break;
            default:
                break;
        }
    }
}



void CFBForwardAI::updatePlayerStates()
{
    CFBPlayerAI::updatePlayerStates();
}



void CFBForwardAI::initPlayerStates()
{
    CFBPlayerAI::initPlayerStates();
    
    m_player->m_isGoalKeeper = false;
}




