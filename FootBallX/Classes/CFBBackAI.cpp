//
//  CFBBackAI.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBBackAI.h"
#include "CFBMatch.h"
#include "CRandomManager.h"

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
        case FBDefs::AI_STATE::CONTROL:
            updateAIControlBall(dt);
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



void CFBBackAI::thinkControlBall()
{
    thinkDribbleBall();
    thinkPassBall();
}



void CFBBackAI::updateAIControlBall(float dt)
{
    switch (m_controlState)
    {
        case FBDefs::AI_STATE_CONTROL::DRIBBLE:
            if (isOnPosition(m_moveToTarget))
            {
                m_controlState = FBDefs::AI_STATE_CONTROL::NONE;
            }
            else
            {
                moveTo(m_moveToTarget, dt);
            }
            break;
        default:
            break;
    }
}

