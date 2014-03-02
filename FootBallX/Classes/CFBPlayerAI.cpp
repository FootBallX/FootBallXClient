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
        m_homePosition.setPoint(posX, posY);
        m_origHomePosition = m_homePosition;
        
        m_defendOrbitRadius = pitch->transformPersentageX(radius);
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
    
    updatePlayerStates();
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
    m_player->m_curPosition = pitch->transformPersentage(m_homePosition, team->getSide());
    m_player->m_isOnDuty = true;
    m_state = FBDefs::AI_STATE::BACKHOME;
}



void CFBPlayerAI::returnToHome(float dt)
{
    CC_ASSERT(m_player);
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    auto homePos = pitch->transformPersentage(m_homePosition, team->getSide());
    if (!isOnPosition(homePos))
    {
        moveTo(homePos, dt);
    }
}



void CFBPlayerAI::moveTo(const Point& pos, float dt)
{
    auto v = (pos - m_player->m_curPosition).normalize();
    m_player->m_curPosition += v * m_player->getSpeed() * dt;
}



void CFBPlayerAI::chaseBall(float dt)
{
    CC_ASSERT(m_player);
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    auto homePos = pitch->transformPersentage(m_homePosition, team->getSide());
    
    if (m_formation->getTeam()->isDefending())
    {
        auto ballPos = FBMATCH->getBall()->getBallPos();
        
        auto ls = (homePos - m_player->m_curPosition).getLengthSq();
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
    auto pt = m_player->m_curPosition - pos;
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
        const Point& ballPos = FBMATCH->getBall()->getBallPos();
        auto pitch = FBMATCH->getPitch();
        auto team = m_formation->getTeam();
        auto homePos = pitch->transformPersentage(m_homePosition, team->getSide());

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



void CFBPlayerAI::PreventOffsideInRate(float& x)
{
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    FBDefs::SIDE side = team->getSide();
    FBDefs::SIDE otherSide = pitch->getOtherSide(side);
    float offsideLine = FBMATCH->getTeam(otherSide)->getLastPosOfPlayer();
    offsideLine = 1 - pitch->transformToPersentageX(offsideLine, otherSide);
    
    if (x > offsideLine)
    {
        x = offsideLine;
    }
}



void CFBPlayerAI::PreventOffside(float& x)
{
    auto pitch = FBMATCH->getPitch();
    auto team = m_formation->getTeam();
    FBDefs::SIDE side = team->getSide();
    x = pitch->transformToPersentageX(x, side);
    PreventOffsideInRate(x);
    x = pitch->transformPersentageX(x, side);
}


#pragma mark ----- Goalkeeper AI
void CFBGoalkeeperAI::thinkHomePosition()
{
    auto pitch = FBMATCH->getPitch();
    auto ball = FBMATCH->getBall();
    
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto ballPos = ball->getBallPos();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = yRate * .2;

    m_homePosition.y = m_origHomePosition.y + yOffset;
    m_homePosition.x = m_origHomePosition.x;
}



void CFBGoalkeeperAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    
    switch (m_state)
    {
        case FBDefs::AI_STATE::BACKHOME:
            returnToHome(dt);
            break;
        case FBDefs::AI_STATE::WAIT:
            updateWait(dt);
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

void CFBBackAI::thinkHomePosition()
{
    auto team = m_formation->getTeam();
    auto pitch = FBMATCH->getPitch();
    auto ball = FBMATCH->getBall();
    
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto ballPos = ball->getBallPos();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = yRate * .2;
    m_homePosition.y = m_origHomePosition.y + yOffset;
    
    FBDefs::SIDE side = team->getSide();
    float ballRate = FBMATCH->getBallPosRateBySide(side);
    
    if (team->isAttacking())
    {
        if (FBMATCH->isBallOnTheSide(side))
        {
            ballRate = ballRate / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::ATK_DEF_BACK_LINE_MIN + (FBDefs::ATK_DEF_BACK_LINE_MAX - FBDefs::ATK_DEF_BACK_LINE_MIN) * ballRate;
        }
        else
        {
            ballRate = 1 - (1 - ballRate) / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::ATK_ATK_BACK_LINE_MIN + (FBDefs::ATK_ATK_BACK_LINE_MAX - FBDefs::ATK_ATK_BACK_LINE_MIN) * ballRate;
        }
    }
    else
    {
        if (FBMATCH->isBallOnTheSide(side))
        {
            ballRate = ballRate / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::DEF_DEF_BACK_LINE_MIN + (FBDefs::DEF_DEF_BACK_LINE_MAX - FBDefs::DEF_DEF_BACK_LINE_MIN) * ballRate;
        }
        else
        {
            ballRate = 1 - (1 - ballRate) / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::DEF_ATK_BACK_LINE_MIN + (FBDefs::DEF_ATK_BACK_LINE_MAX - FBDefs::DEF_ATK_BACK_LINE_MIN) * ballRate;
        }
    }
}



void CFBBackAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    if (!this->m_player->m_isBallController)
    {
        switch (m_state)
        {
            case FBDefs::AI_STATE::BACKHOME:
                returnToHome(dt);
                break;
            case FBDefs::AI_STATE::WAIT:
                updateWait(dt);
                break;
            case FBDefs::AI_STATE::CHASE:
                chaseBall(dt);
                break;
            default:
                break;
        }
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
    if (!this->m_player->m_isBallController)
    {
        switch (m_state)
        {
            case FBDefs::AI_STATE::BACKHOME:
                returnToHome(dt);
                break;
            case FBDefs::AI_STATE::WAIT:
                updateWait(dt);
                break;
            case FBDefs::AI_STATE::CHASE:
                chaseBall(dt);
                break;
            default:
                break;
        }
    }
}



void CFBHalfBackAI::thinkHomePosition()
{
    auto team = m_formation->getTeam();
    auto pitch = FBMATCH->getPitch();
    auto ball = FBMATCH->getBall();
    
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto ballPos = ball->getBallPos();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = yRate * .2;
    m_homePosition.y = m_origHomePosition.y + yOffset;
    
    FBDefs::SIDE side = team->getSide();
    float ballRate = FBMATCH->getBallPosRateBySide(side);
    
    if (team->isAttacking())
    {
        if (FBMATCH->isBallOnTheSide(side))
        {
            ballRate = ballRate / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::ATK_DEF_HALF_BACK_LINE_MIN + (FBDefs::ATK_DEF_HALF_BACK_LINE_MAX - FBDefs::ATK_DEF_HALF_BACK_LINE_MIN) * ballRate;
        }
        else
        {
            ballRate = 1 - (1 - ballRate) / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::ATK_ATK_HALF_BACK_LINE_MIN + (FBDefs::ATK_ATK_HALF_BACK_LINE_MAX - FBDefs::ATK_ATK_HALF_BACK_LINE_MIN) * ballRate;
        }
    }
    else
    {
        if (FBMATCH->isBallOnTheSide(side))
        {
            ballRate = ballRate / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::DEF_DEF_HALF_BACK_LINE_MIN + (FBDefs::DEF_DEF_HALF_BACK_LINE_MAX - FBDefs::DEF_DEF_HALF_BACK_LINE_MIN) * ballRate;
        }
        else
        {
            ballRate = 1 - (1 - ballRate) / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::DEF_ATK_HALF_BACK_LINE_MIN + (FBDefs::DEF_ATK_HALF_BACK_LINE_MAX - FBDefs::DEF_ATK_HALF_BACK_LINE_MIN) * ballRate;
        }
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
                returnToHome(dt);
                break;
            case FBDefs::AI_STATE::WAIT:
                updateWait(dt);
                break;
            case FBDefs::AI_STATE::CHASE:
                chaseBall(dt);
                break;
            case FBDefs::AI_STATE::SUPPORT:
                this->updateSupport(dt);
                break;
            default:
                break;
        }
    }
}



void CFBForwardAI::thinkHomePosition()
{
    auto team = m_formation->getTeam();
    auto pitch = FBMATCH->getPitch();
    auto ball = FBMATCH->getBall();
    
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto ballPos = ball->getBallPos();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = yRate * .2;
    m_homePosition.y = m_origHomePosition.y + yOffset;
    
    FBDefs::SIDE side = team->getSide();
    float ballRate = FBMATCH->getBallPosRateBySide(side);
    
    if (team->isAttacking())
    {
        if (FBMATCH->isBallOnTheSide(side))
        {
            ballRate = ballRate / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::ATK_DEF_FORWORD_LINE_MIN + (FBDefs::ATK_DEF_FORWORD_LINE_MAX - FBDefs::ATK_DEF_FORWORD_LINE_MIN) * ballRate;
        }
        else
        {
            ballRate = 1 - (1 - ballRate) / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::ATK_ATK_FORWORD_LINE_MIN + (FBDefs::ATK_ATK_FORWORD_LINE_MAX - FBDefs::ATK_ATK_FORWORD_LINE_MIN) * ballRate;
        }
    }
    else
    {
        if (FBMATCH->isBallOnTheSide(side))
        {
            ballRate = ballRate / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::DEF_DEF_FORWORD_LINE_MIN + (FBDefs::DEF_DEF_FORWORD_LINE_MAX - FBDefs::DEF_DEF_FORWORD_LINE_MIN) * ballRate;
        }
        else
        {
            ballRate = 1 - (1 - ballRate) / 0.5f;
            m_homePosition.x = m_origHomePosition.x + FBDefs::DEF_ATK_FORWORD_LINE_MIN + (FBDefs::DEF_ATK_FORWORD_LINE_MAX - FBDefs::DEF_ATK_FORWORD_LINE_MIN) * ballRate;
        }
    }
    
    PreventOffsideInRate(m_homePosition.x);
}



void CFBForwardAI::updatePlayerStates()
{
    CFBPlayerAI::updatePlayerStates();
    
    if (this->m_player->m_ownerTeam->isAttacking())
    {
        if (this->m_player->m_isBallController)
        {
            
        }
        else
        {
            auto team = m_formation->getTeam();
            FBDefs::SIDE side = team->getSide();
            
            if (team->isAttacking())
            {
                if (FBMATCH->isBallOnTheSide(side))
                {
                    this->m_state = FBDefs::AI_STATE::BACKHOME;
                }
                else
                {
                    if (this->m_state != FBDefs::AI_STATE::SUPPORT && this->m_state != FBDefs::AI_STATE::WAIT)
                    {
                        this->m_state = FBDefs::AI_STATE::SUPPORT;
                        this->m_supportState = SUPPORT_STATE::FIND_POS;
                    }
                }
            }
        }
    }
    else // defending
    {
        
    }
}



void CFBForwardAI::initPlayerStates()
{
    CFBPlayerAI::initPlayerStates();
    
    m_player->m_isGoalKeeper = false;
}



void CFBForwardAI::updateSupport(float dt)
{
    switch (m_supportState)
    {
        case SUPPORT_STATE::FIND_POS:
        {
            auto pitch = FBMATCH->getPitch();
            m_moveToTarget = pitch->getBestSupportPosition(m_formation->getTeam()->getSide());
            m_supportState = SUPPORT_STATE::MOVE_TO_POS;
            PreventOffside(m_moveToTarget.x);
            break;
        }
        case SUPPORT_STATE::MOVE_TO_POS:
        {
            if (!isOnPosition(m_moveToTarget))
            {
                moveTo(m_moveToTarget, dt);
            }
            else
            {
                startWait(2.f);
            }
            break;
        }
    }

}

