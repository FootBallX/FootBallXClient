//
//  CFBForwardAI.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBForwardAI.h"
#include "CFBMatch.h"
#include "CRandomManager.h"

bool CFBForwardAI::init(CFBFormation* formation, CFBPlayer* player, const cocos2d::Point& intPos, const cocos2d::Point& homePos, float radius, bool networkControl)
{
    do
    {
        BREAK_IF_FAILED(CFBPlayerAI::init(formation, player, intPos, homePos, radius, networkControl));
        m_player->m_isGoalKeeper = false;
        
        return true;
    } while (false);
    
    return false;
}

void CFBForwardAI::update(float dt)
{
    CFBPlayerAI::update(dt);

//    switch (m_state)
//    {
//        case FBDefs::AI_STATE::BACKHOME:
//            returnToHome(dt);
//            break;
//        case FBDefs::AI_STATE::WAIT:
//            updateWait(dt);
//            break;
//        case FBDefs::AI_STATE::CHASE:
//            chaseBall(dt);
//            break;
//        case FBDefs::AI_STATE::SUPPORT:
//            this->updateSupport(dt);
//            break;
//        case FBDefs::AI_STATE::AI_CONTROL:
//            updateAIControlBall(dt);
//            break;
//        default:
//            break;
//    }
}



void CFBForwardAI::updateHomePosition()
{
    auto team = m_formation->getTeam();
    auto pitch = FBMATCH->getPitch();
    
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto& ballPos = FBMATCH->getBallPosition();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = yRate * FBDefs::OFFSET_Y;
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
    
    PreventOffside(m_homePosition.x);
}



void CFBForwardAI::considerSupport()
{
    auto team = m_formation->getTeam();
    if (m_state == FBDefs::AI_STATE::BACKHOME)
    {
        auto side = team->getSide();
        if (!FBMATCH->isBallOnTheSide(side))
        {
            if (this->m_state != FBDefs::AI_STATE::SUPPORT && this->m_state != FBDefs::AI_STATE::WAIT)
            {
                this->m_state = FBDefs::AI_STATE::SUPPORT;
                this->m_supportState = FBDefs::AI_STATE_SUPPORT::FIND_POS;
            }
        }
    }
}



void CFBForwardAI::updateSupport(float dt)
{
    switch (m_supportState)
    {
        case FBDefs::AI_STATE_SUPPORT::FIND_POS:
        {
            auto pitch = FBMATCH->getPitch();
            m_moveToTarget = pitch->getBestSupportPosition(m_formation->getTeam()->getSide());
            m_supportState = FBDefs::AI_STATE_SUPPORT::MOVE_TO_POS;
            PreventOffside(m_moveToTarget.x);
            break;
        }
        case FBDefs::AI_STATE_SUPPORT::MOVE_TO_POS:
        {
            if (m_player->moveTo(m_moveToTarget))
            {
                startWait(2.f);
            }
            break;
        }
        default:
            break;
    }
    
}



void CFBForwardAI::thinkControlBall()
{
    thinkDribbleBall();
    thinkPassBall();
}



void CFBForwardAI::updateAIControlBall(float dt)
{
    switch (m_controlState)
    {
        case FBDefs::AI_STATE_CONTROL::DRIBBLE:
            if (m_player->moveTo(m_moveToTarget))
            {
                m_controlState = FBDefs::AI_STATE_CONTROL::NONE;
            }
            break;
        default:
            break;
    }
}

