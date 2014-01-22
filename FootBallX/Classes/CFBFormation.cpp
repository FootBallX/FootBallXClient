//
//  CFBFormation.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBFormation.h"

#pragma mark ----- FormationBase

CFBFormation::~CFBFormation()
{
    for (int i = 0; i < m_playerNumber; ++i)
    {
        CC_SAFE_DELETE(m_playerAIs[i]);
    }
    CC_SAFE_DELETE_ARRAY(m_playerAIs);
}

bool CFBFormation::init()
{
    do
    {
        CC_SAFE_DELETE_ARRAY(m_playerAIs);
        
        m_playerAIs = new CFBPlayerAI*[m_playerNumber];
        
        return true;
    } while (false);
    
    return false;
}


void CFBFormation::update(float dt)
{
    CC_ASSERT(m_playerAIs);
    for (int i = 0; i < m_playerNumber; ++i)
    {
        CC_ASSERT(m_playerAIs[i]);
        m_playerAIs[i]->update(dt);
    }
}



bool CFBFormation::onStartMatch()
{
    do
    {
        CC_ASSERT(m_playerAIs);
        for (int i = 0; i < m_playerNumber; ++i)
        {
            CC_ASSERT(m_playerAIs[i]);
            m_playerAIs[i]->initPlayerStates();
        }
        
        return true;
    } while (false);
    
    return false;
}


#pragma mark ----- 442

CFBFormation442::CFBFormation442()
{
    m_playerNumber = 11;
}

const char* CFBFormation442::description()
{
    return "4-4-2";
}

bool CFBFormation442::init()
{
    do
    {
        BREAK_IF_FAILED(CFBFormation::init());

        m_playerAIs[0] = new CFBGoalkeeperAI();
        m_playerAIs[1] = new CFBBackAI();
        m_playerAIs[2] = new CFBBackAI();
        m_playerAIs[3] = new CFBBackAI();
        m_playerAIs[4] = new CFBBackAI();
        m_playerAIs[5] = new CFBHalfBackAI();
        m_playerAIs[6] = new CFBHalfBackAI();
        m_playerAIs[7] = new CFBHalfBackAI();
        m_playerAIs[8] = new CFBHalfBackAI();
        m_playerAIs[9] = new CFBForwardAI();
        m_playerAIs[10] = new CFBForwardAI();
        return true;
    } while (false);
	return false;
}



void CFBFormation442::addPlayer(CFBPlayer* player, int pos)
{
    switch (pos)
    {
        case 0:
            m_playerAIs[pos]->init(player, 0.05f, 0.5f, GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            m_playerAIs[pos]->init(player, 0.15f, 0.8f, BACK_ORBIT_RATE);
            break;
        case 2:
            m_playerAIs[pos]->init(player, 0.15f, 0.6f, BACK_ORBIT_RATE);
            break;
        case 3:
            m_playerAIs[pos]->init(player, 0.15f, 0.4f, BACK_ORBIT_RATE);
            break;
        case 4:
            m_playerAIs[pos]->init(player, 0.15f, 0.2f, BACK_ORBIT_RATE);
            break;
        case 5:
            m_playerAIs[pos]->init(player, 0.5f, 0.8f, HALF_BACK_ORBIT_RATE);
            break;
        case 6:
            m_playerAIs[pos]->init(player, 0.5f, 0.6f, HALF_BACK_ORBIT_RATE);
            break;
        case 7:
            m_playerAIs[pos]->init(player, 0.5f, 0.4f, HALF_BACK_ORBIT_RATE);
            break;
        case 8:
            m_playerAIs[pos]->init(player, 0.5f, 0.2f, HALF_BACK_ORBIT_RATE);
            break;
        case 9:
            m_playerAIs[pos]->init(player, 0.8f, 0.667f, FORWARD_ORBIT_RATE);
            break;
        case 10:
            m_playerAIs[pos]->init(player, 0.8f, 0.333f, FORWARD_ORBIT_RATE);
            break;
        default:
            CC_ASSERT(false);
            break;
    }
}

#pragma mark ----- 352

CFBFormation352::CFBFormation352()
{
    m_playerNumber = 11;
}


const char* CFBFormation352::description()
{
    return "3-5-2";
}


bool CFBFormation352::init()
{
    do
    {
        BREAK_IF_FAILED(CFBFormation::init());
        
        m_playerAIs[0] = new CFBGoalkeeperAI();
        m_playerAIs[1] = new CFBBackAI();
        m_playerAIs[2] = new CFBBackAI();
        m_playerAIs[3] = new CFBBackAI();
        m_playerAIs[4] = new CFBHalfBackAI();
        m_playerAIs[5] = new CFBHalfBackAI();
        m_playerAIs[6] = new CFBHalfBackAI();
        m_playerAIs[7] = new CFBHalfBackAI();
        m_playerAIs[8] = new CFBHalfBackAI();
        m_playerAIs[9] = new CFBForwardAI();
        m_playerAIs[10] = new CFBForwardAI();
        
        return true;
    } while (false);
	return false;
}



void CFBFormation352::addPlayer(CFBPlayer* player, int pos)
{
    switch (pos)
    {
        case 0:
            m_playerAIs[pos]->init(player, 0.05f, 0.5f, GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            m_playerAIs[pos]->init(player, 0.15f, 0.75f, BACK_ORBIT_RATE);
            break;
        case 2:
            m_playerAIs[pos]->init(player, 0.15f, 0.5f, BACK_ORBIT_RATE);
            break;
        case 3:
            m_playerAIs[pos]->init(player, 0.15f, 0.25f, BACK_ORBIT_RATE);
            break;
        case 4:
            m_playerAIs[pos]->init(player, 0.4f, 0.65f, HALF_BACK_ORBIT_RATE);
            break;
        case 5:
            m_playerAIs[pos]->init(player, 0.4f, 0.35f, HALF_BACK_ORBIT_RATE);
            break;
        case 6:
            m_playerAIs[pos]->init(player, 0.6f, 0.75f, HALF_BACK_ORBIT_RATE);
            break;
        case 7:
            m_playerAIs[pos]->init(player, 0.6f, 0.5f, HALF_BACK_ORBIT_RATE);
            break;
        case 8:
            m_playerAIs[pos]->init(player, 0.6f, 0.25f, HALF_BACK_ORBIT_RATE);
            break;
        case 9:
            m_playerAIs[pos]->init(player, 0.8f, 0.667f, FORWARD_ORBIT_RATE);
            break;
        case 10:
            m_playerAIs[pos]->init(player, 0.8f, 0.333f, FORWARD_ORBIT_RATE);
            break;
        default:
            CC_ASSERT(false);
            break;
    }
}

