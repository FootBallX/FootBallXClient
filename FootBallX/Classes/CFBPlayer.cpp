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
    if (!m_targetPosition.equals(Point(0, 0)))
    {
        if (FBDefs::isPitchPointAlmostSame(m_curPosition, m_targetPosition))
        {
            m_movingVector.setPoint(0, 0);
        }
    }
    
    auto spd = this->getSpeed();
    m_curPosition += m_movingVector * spd * dt;
    if (m_isBallController)
    {
        FBMATCH->setBallPosition(m_curPosition);
    }
}



void CFBPlayer::gainBall()
{
    CC_ASSERT(m_isOnDuty);
    
    m_isBallController = true;
    
    FBMATCH->setBallPosition(getPosition());

    m_ownerTeam->setAttacking(true);
    
    m_ownerTeam->setHilightPlayerId(this->m_positionInFormation);
    
    m_movingVector.setPoint(0, 0);
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

    return m_speedCache * m_speedScale;
}



void CFBPlayer::setPosition(const Point& pos)
{
    m_curPosition = pos;
    if (m_isBallController)
    {
        FBMATCH->setBallPosition(pos);
    }
}



bool CFBPlayer::moveTo(const Point& pos, float dt)
{
    m_targetPosition = pos;
    
    if (FBDefs::isPitchPointAlmostSame(m_curPosition, pos))
    {
        m_movingVector.setPoint(0, 0);
        return true;
    }
    else
    {
        if (dt < 0)
        {
            dt = 0;
        }
        
        m_movingVector = (pos - m_curPosition).normalize();
        m_curPosition += m_movingVector * (dt * getSpeed());
        return false;
    }
}



bool CFBPlayer::moveFromTo(const cocos2d::Point& pos, const cocos2d::Point& vec, float dt, float duration)
{
    Point target = pos + vec * (getSpeed() * duration);
    
    // TODO: 考虑加入速度变化，使得moveTo的过程更加精确。
    moveTo(target);
    
    return false;
}



void CFBPlayer::setMovingVector(const cocos2d::Point& vec)
{
    m_targetPosition.setPoint(0, 0);
    m_movingVector = vec;
}



void CFBPlayer::setMovingVector(float x, float y)
{
    m_targetPosition.setPoint(0, 0);
    m_movingVector.setPoint(x, y);
}


