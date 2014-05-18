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
    m_playerAIs.clear();
}

bool CFBFormation::init(CFBTeam* team)
{
    do
    {
        BREAK_IF(team == nullptr);
        m_team = team;
        
//        CC_SAFE_DELETE_ARRAY(m_playerAIs);
//        
//        m_playerAIs = new CFBPlayerAI*[m_playerNumber];
//        m_playerIndices.resize(m_playerNumber);
//        iota(m_playerIndices.begin(), m_playerIndices.end(), 0);
        
        return true;
    } while (false);
    
    return false;
}


void CFBFormation::update(float dt)
{
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



bool CFBFormation::onStartMatch(const vector<CFBPlayerInitInfo>& initPlayerInfo, const vector<CFBPlayer*>& players, bool networkControl)
{
    do
    {
        float orbit;
        m_playerNumber = (int)initPlayerInfo.size();
        for (int i = 0; i < m_playerNumber; ++i)
        {
            switch (initPlayerInfo[i].aiClass)
            {
                case 0:
                    m_playerAIs.push_back(new CFBGoalkeeperAI());
                    orbit = GOALKEEPER_ORBIT_RATE;
                    break;
                case 1:
                    m_playerAIs.push_back(new CFBBackAI());
                    orbit = BACK_ORBIT_RATE;
                    break;
                case 2:
                    m_playerAIs.push_back(new CFBHalfBackAI());
                    orbit = HALF_BACK_ORBIT_RATE;
                    break;
                case 3:
                    m_playerAIs.push_back(new CFBForwardAI());
                    orbit = FORWARD_ORBIT_RATE;
                    break;
                default:
                    CC_ASSERT(false);
                    break;
            }
            
            players[i]->m_positionInFormation = i;
            m_playerAIs[i]->init(this, players[i], initPlayerInfo[i].position, initPlayerInfo[i].homePosition, orbit, networkControl);
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



CFBPlayer* CFBFormation::getKickOffPlayer()
{
    // TODO: 这里要改，大概应该从服务器指定开球队员吧。
    return getPlayer(9);
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

