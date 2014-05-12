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
#include "CFBGoalkeeperAI.h"
#include "CFBBackAI.h"
#include "CFBHalfBackAI.h"
#include "CFBForwardAI.h"

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
    
//    m_updateCD++;
//    int thinkIndex = int(m_updateCD) % m_playerNumber;
//    if (thinkIndex != m_updateIndex)
//    {
//        m_updateIndex = thinkIndex;
//        if (thinkIndex == 0)
//        {
//            random_shuffle(m_playerIndices.begin(), m_playerIndices.end());
//        }
//        
//        m_playerAIs[m_playerIndices[thinkIndex]]->think();
//    }
    
    for (int i = 0; i < m_playerNumber; ++i)
    {
        CC_ASSERT(m_playerAIs[i]);
        m_playerAIs[i]->update(dt);
    }
}



void CFBFormation::think()
{
    for (int i = 0; i < m_playerNumber; ++i)
    {
        CC_ASSERT(m_playerAIs[i]);
        m_playerAIs[i]->think();
    }
}



bool CFBFormation::onStartMatch(const vector<float>& initPlayerPos, bool networkControl)
{
    do
    {
        CC_ASSERT(m_playerAIs);
        for (int i = 0; i < m_playerNumber; ++i)
        {
            CC_ASSERT(m_playerAIs[i]);
            m_playerAIs[i]->initPlayerStates(Point(initPlayerPos[i*2], initPlayerPos[i*2 + 1]), networkControl);
        }
        
        m_team->setHilightPlayerId(m_playerNumber - 1);
        
        return true;
    } while (false);
    
    return false;
}



CFBPlayer* CFBFormation::getPlayer(int pos)
{
    CC_ASSERT(pos >= 0 && pos < m_playerNumber);
    return m_playerAIs[pos]->getPlayer();
}



CFBPlayerAI* CFBFormation::getPassBallTarget()
{
    int max = INT_MIN;
    CFBPlayerAI* player = nullptr;
    for (int i = 0; i < m_playerNumber; ++i)
    {
        int score = m_playerAIs[i]->getPassBallScore();
        if (score > max)
        {
            max = score;
            player = m_playerAIs[i];
        }
    }
    
    return player;
}



//Point CFBFormation::getDribbleTargetPos()
//{
//    auto pitch = FBMATCH->getPitch();
//    auto& grids = pitch->getGrids();
//    
//    int max = INT_MIN;
//    int index = -1;
//    for (auto g : grids)
//    {
//        if (g.m_defenceScore > max)
//        {
//            max = g.m_defenceScore;
//            index = g.m_index;
//        }
//    }
//    
//    CC_ASSERT(index != -1);
//    
//    return pitch->getGrid(index).m_position;
//}


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

        return true;
    } while (false);
	return false;
}



void CFBFormation442::addPlayer(CFBPlayer* player, int pos)
{
    auto initPos = FBDefs::InitFormation[(int)FBDefs::FORMATION::F_4_4_2];
    auto homePos = FBDefs::HomeFormation[(int)FBDefs::FORMATION::F_4_4_2];
    
    switch (pos)
    {
        case 0:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 2:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 3:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 4:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 5:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 6:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 7:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 8:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 9:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], FORWARD_ORBIT_RATE);
            break;
        case 10:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], FORWARD_ORBIT_RATE);
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
    m_formationId = FBDefs::FORMATION::F_3_2_3_2;
}


const char* CFBFormation352::description()
{
    return "3-2_3-2";
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
    auto initPos = FBDefs::InitFormation[(int)FBDefs::FORMATION::F_3_2_3_2];
    auto homePos = FBDefs::HomeFormation[(int)FBDefs::FORMATION::F_3_2_3_2];
    switch (pos)
    {
        case 0:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], GOALKEEPER_ORBIT_RATE);
            break;
        case 1:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 2:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 3:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], BACK_ORBIT_RATE);
            break;
        case 4:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 5:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 6:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 7:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 8:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], HALF_BACK_ORBIT_RATE);
            break;
        case 9:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], FORWARD_ORBIT_RATE);
            break;
        case 10:
            m_playerAIs[pos]->init(this, player, initPos[pos], homePos[pos], FORWARD_ORBIT_RATE);
            break;
        default:
            CC_ASSERT(false);
            break;
    }
    
    player->m_positionInFormation = pos;
}


