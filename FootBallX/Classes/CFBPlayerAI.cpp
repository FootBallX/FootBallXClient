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

#pragma mark ----- AI Base

bool CFBPlayerAI::init(CFBPlayer* player, float posX, float posY, float radius)
{
    do
    {
        BREAK_IF(player == nullptr);
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
    m_player->m_curPosition = FBMATCH->getPitch()->transformPersentage(m_position);
}



#pragma mark ----- Goalkeeper AI

void CFBGoalkeeperAI::update(float dt)
{
    
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




