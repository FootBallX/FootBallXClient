//
//  CFBPlayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPlayer.h"
#include "CFBMatch.h"
#include "CFBCardManager.h"
#include "CFBFunctionsJS.h"

CFBPlayer::CFBPlayer(const string& cid) : m_playerCard(CARD_MGR->getCardById(cid))
{
}



CFBPlayer::CFBPlayer(const CFBCard& card) : m_playerCard(card)
{
}



void CFBPlayer::update(float dt)
{
    if (m_isBallController)
    {
        auto ball = FBMATCH->getBall();
        CC_ASSERT(ball->m_ownerPlayer == this);
        
        ball->setBallPos(m_curPosition);
    }
}

void CFBPlayer::gainBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = true;
    
    FBMATCH->getBall()->m_ownerPlayer = this;
    FBMATCH->getBall()->m_ownerTeam = m_ownerTeam;
    m_ownerTeam->setAttacking(true);
}



void CFBPlayer::loseBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = false;
    
    FBMATCH->getBall()->m_ownerPlayer = nullptr;
    FBMATCH->getBall()->m_ownerTeam = nullptr;
    m_ownerTeam->setAttacking(false);
}



float CFBPlayer::getSpeed()
{
    if (m_speedCache < 0)
    {
        auto p = FBMATCH->getPitch();
        float speed = FB_FUNC_JS->getSpeed(m_playerCard);
        m_speedCache = p->transformPersentageX(speed);
    }

    return m_speedCache;
}



