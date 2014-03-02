//
//  CFBFormation.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBFormation.h"
#include "CFBPlayer.h"
#include "CFBTeam.h"
#include "CFBMatch.h"

#pragma mark ----- FormationBase

CFBFormation::~CFBFormation()
{
    for (int i = 0; i < m_playerNumber; ++i)
    {
        CC_SAFE_DELETE(m_playerAIs[i]);
    }
    CC_SAFE_DELETE_ARRAY(m_playerAIs);
}

bool CFBFormation::init(CFBTeam* team)
{
    do
    {
        BREAK_IF(team == nullptr);
        m_team = team;
        
        CC_SAFE_DELETE_ARRAY(m_playerAIs);
        
        m_playerAIs = new CFBPlayerAI*[m_playerNumber];
        m_playerIndices.resize(m_playerNumber);
        iota(m_playerIndices.begin(), m_playerIndices.end(), 0);
        
        return true;
    } while (false);
    
    return false;
}


void CFBFormation::update(float dt)
{
    CC_ASSERT(m_playerAIs);
    
//    update3Lines(dt);

    m_updateCD++;
    int thinkIndex = int(m_updateCD) % m_playerNumber;
    if (thinkIndex != m_updateIndex)
    {
        m_updateIndex = thinkIndex;
        if (thinkIndex == 0)
        {
            random_shuffle(m_playerIndices.begin(), m_playerIndices.end());
        }
        
        m_playerAIs[m_playerIndices[thinkIndex]]->think();
    }
    
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



CFBPlayer* CFBFormation::getPlayer(int pos)
{
    CC_ASSERT(pos >= 0 && pos < m_playerNumber);
    return m_playerAIs[pos]->getPlayer();
}



#pragma mark ----- 442

CFBFormation442::CFBFormation442()
{
    m_playerNumber = 11;
    m_formationId = FBDefs::FORMATION::F_4_4_2;
}

const char* CFBFormation442::description()
{
    return "4-4-2";
}

bool CFBFormation442::init(CFBTeam* team)
{
    do
    {
        BREAK_IF_FAILED(CFBFormation::init(team));

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
        
        auto pitch = FBMATCH->getPitch();
        auto side = team->getSide();

        return true;
    } while (false);
	return false;
}



void CFBFormation442::addPlayer(CFBPlayer* player, int pos)
{
    player->m_positionInFormation = pos;
    switch (pos)
    {
        case 0:
            m_playerAIs[pos]->init(this, player, 0.f, 0.5f, GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            m_playerAIs[pos]->init(this, player, 0.f, 0.8f, BACK_ORBIT_RATE);
            break;
        case 2:
            m_playerAIs[pos]->init(this, player, 0.f, 0.6f, BACK_ORBIT_RATE);
            break;
        case 3:
            m_playerAIs[pos]->init(this, player, 0.f, 0.4f, BACK_ORBIT_RATE);
            break;
        case 4:
            m_playerAIs[pos]->init(this, player, 0.f, 0.2f, BACK_ORBIT_RATE);
            break;
        case 5:
            m_playerAIs[pos]->init(this, player, 0.f, 0.8f, HALF_BACK_ORBIT_RATE);
            break;
        case 6:
            m_playerAIs[pos]->init(this, player, 0.f, 0.6f, HALF_BACK_ORBIT_RATE);
            break;
        case 7:
            m_playerAIs[pos]->init(this, player, 0.f, 0.4f, HALF_BACK_ORBIT_RATE);
            break;
        case 8:
            m_playerAIs[pos]->init(this, player, 0.f, 0.2f, HALF_BACK_ORBIT_RATE);
            break;
        case 9:
            m_playerAIs[pos]->init(this, player, 0.f, 0.667f, FORWARD_ORBIT_RATE);
            break;
        case 10:
            m_playerAIs[pos]->init(this, player, 0.f, 0.333f, FORWARD_ORBIT_RATE);
            break;
        default:
            CC_ASSERT(false);
            break;
    }
    
    player->m_positionInFormation = pos;
}



#pragma mark ----- 352

CFBFormation352::CFBFormation352()
{
    m_playerNumber = 11;
    m_formationId = FBDefs::FORMATION::F_3_5_2;
}


const char* CFBFormation352::description()
{
    return "3-5-2";
}


bool CFBFormation352::init(CFBTeam* team)
{
    do
    {
        BREAK_IF_FAILED(CFBFormation::init(team));
        
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
            m_playerAIs[pos]->init(this, player, 0.f, 0.5f, GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            m_playerAIs[pos]->init(this, player, 0.f, 0.75f, BACK_ORBIT_RATE);
            break;
        case 2:
            m_playerAIs[pos]->init(this, player, 0.f, 0.5f, BACK_ORBIT_RATE);
            break;
        case 3:
            m_playerAIs[pos]->init(this, player, 0.f, 0.25f, BACK_ORBIT_RATE);
            break;
        case 4:
            m_playerAIs[pos]->init(this, player, -0.05f, 0.65f, HALF_BACK_ORBIT_RATE);
            break;
        case 5:
            m_playerAIs[pos]->init(this, player, -0.05f, 0.35f, HALF_BACK_ORBIT_RATE);
            break;
        case 6:
            m_playerAIs[pos]->init(this, player, 0.05f, 0.75f, HALF_BACK_ORBIT_RATE);
            break;
        case 7:
            m_playerAIs[pos]->init(this, player, 0.05f, 0.5f, HALF_BACK_ORBIT_RATE);
            break;
        case 8:
            m_playerAIs[pos]->init(this, player, 0.05f, 0.25f, HALF_BACK_ORBIT_RATE);
            break;
        case 9:
            m_playerAIs[pos]->init(this, player, 0.f, 0.667f, FORWARD_ORBIT_RATE);
            break;
        case 10:
            m_playerAIs[pos]->init(this, player, 0.f, 0.333f, FORWARD_ORBIT_RATE);
            break;
        default:
            CC_ASSERT(false);
            break;
    }
    
    player->m_positionInFormation = pos;
}



