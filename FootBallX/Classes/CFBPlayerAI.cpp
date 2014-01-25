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
        m_position.setPoint(posX, posY);
        m_radiusOfOrbitRate = radius;

        return true;
    } while (false);
    
    return false;
}



void CFBPlayerAI::updatePlayerStates()
{

}



void CFBPlayerAI::initPlayerStates()
{
    Point pt(m_position.x * 0.5f, m_position.y);        // 初始球员位置局限在本方半场。
    m_player->m_curPosition = FBMATCH->getPitch()->transformPersentage(pt, m_formation->getTeam()->getSide());
    m_player->m_isOnDuty = true;
}



void CFBPlayerAI::returnToPosition(float dt)
{
    CC_ASSERT(m_player);
    Point pos = FBMATCH->getPitch()->transformPersentage(m_position, m_formation->getTeam()->getSide());
    
    if (!pos.equals(m_player->m_curPosition))
    {
        Point v = (pos - m_player->m_curPosition).normalize();
        m_player->m_curPosition += v * FBMATCH->getPitch()->transformPersentage(m_player->m_speed * dt);
    }
}



#pragma mark ----- Goalkeeper AI

void CFBGoalkeeperAI::update(float dt)
{
    returnToPosition(dt);
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
    returnToPosition(dt);
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
    returnToPosition(dt);
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
    returnToPosition(dt);
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




