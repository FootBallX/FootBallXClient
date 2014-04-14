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
}

void CFBPlayer::gainBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = true;
    
    FBMATCH->setBallPosition(getPosition());

    m_ownerTeam->setAttacking(true);
    
    m_ownerTeam->setHilightPlayerId(this->m_positionInFormation);
}



void CFBPlayer::loseBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = false;

    m_ownerTeam->setAttacking(false);
}



float CFBPlayer::getSpeed()
{
    if (m_speedCache < 0)
    {
        float speed = FB_FUNC_JS->getSpeed(m_playerCard);
        m_speedCache = speed;
    }

    return m_speedCache;
}



void CFBPlayer::setPosition(const Point& pos)
{
    m_curPosition = pos;
    if (m_isBallController)
    {
        FBMATCH->setBallPosition(pos);
    }
}

