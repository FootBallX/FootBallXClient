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
#include "CFBGoalkeeperAI.h"
#include "CFBBackAI.h"
#include "CFBHalfBackAI.h"
#include "CFBForwardAI.h"

CFBPlayer::CFBPlayer(CFBTeam* team, const CFBCard& card)
: m_ownerTeam(team)
,m_playerCard(card)
{
}



//CFBPlayer::CFBPlayer(const CFBCard& card) : m_playerCard(card)
//{
//}

bool CFBPlayer::createBrain(FBDefs::AI_CLASS aiClass, const Point& homePos, float orbit)
{
    do
    {
        CC_SAFE_DELETE(m_brain);
        switch (aiClass)
        {
            case FBDefs::AI_CLASS::GOAL_KEEPER:
                m_brain = new CFBGoalkeeperAI();
                break;
            case FBDefs::AI_CLASS::BACK:
                m_brain = new CFBBackAI();
                break;
            case FBDefs::AI_CLASS::HALF_BACK:
                m_brain = new CFBHalfBackAI();
                break;
            case FBDefs::AI_CLASS::FORWARD:
                m_brain = new CFBForwardAI();
                break;
        }
        m_brain->init(m_ownerTeam, this, homePos, orbit);
        return true;
    } while (false);
    
    return false;
}



CFBPlayerAI* CFBPlayer::getBrain()
{
    return m_brain;
}



CFBTeam* CFBPlayer::getOwnerTeam() const
{
    return m_ownerTeam;
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
    
    m_stunTime -= dt;
}



void CFBPlayer::gainBall()
{
    m_isBallController = true;
    
    FBMATCH->setBallPosition(getPosition());

    m_ownerTeam->setAttacking(true);
    
    m_ownerTeam->setHilightPlayerId(this->m_positionInFormation);
    
    m_movingVector.setPoint(0, 0);
}



void CFBPlayer::loseBall()
{
    m_isBallController = false;

    m_ownerTeam->setAttacking(false);
}



float CFBPlayer::getSpeed()
{
    if (m_speedCache < 0)
    {
        float speed = m_playerCard.m_speed;
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



void CFBPlayer::stun()
{
    m_stunTime = FBDefs::STUN_TIME;
}



bool CFBPlayer::isStunned()
{
    return m_stunTime > 0;
}


