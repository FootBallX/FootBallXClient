//
//  CFBTeam.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-17.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBTeam.h"
#include "CFBFormation.h"
#include "CFBPlayer.h"

CFBTeam::CFBTeam()
: m_side(FBDefs::SIDE::NONE)
{
    
}



bool CFBTeam::init()
{
    do
    {
        // player 对象应该从哪里来？服务器？这里暂时创建一下。
        for (int i = 0; i < 20; ++i)
        {
            auto player = new CFBPlayer();
            player->m_ownerTeam = this;
            m_teamMembers.push_back(player);
        }
        m_formation = new CFBFormation442();
        BREAK_IF_FAILED(m_formation->init(this));
        
        m_state = FBDefs::TEAM_STATE::DEFENDING;
        return true;
    } while (false);
    
    return false;
}



void CFBTeam::update(float dt)
{
    m_formation->update(dt);
}



bool CFBTeam::onStartMatch()
{
    do
    {
        CC_ASSERT(m_teamMembers.size() >= m_formation->getPlayerNumber());
        
        for (int i = 0; i < m_formation->getPlayerNumber(); ++i)
        {
            m_formation->addPlayer(m_teamMembers[i], i);
        }
        
        BREAK_IF_FAILED(m_formation->onStartMatch());
        
        return true;
    } while (false);
    
    return false;
}



void CFBTeam::kickOff()
{
    auto player = m_formation->getKickOffPlayer();
    player->gainBall();
    m_state = FBDefs::TEAM_STATE::ATTACKING;
}



CFBPlayer* CFBTeam::getPlayingPlayer()
{
    for (int i = 0; i < m_teamMembers.size(); ++i)
    {
        auto player = m_teamMembers[i];
        if (player->m_isOnDuty && player->m_isBallController)
        {
            return player;
        }
    }
    
    return nullptr;
}



bool CFBTeam::changeFormation(FBDefs::FORMATION formationId)
{
    if (formationId == m_formation->getFormationId()) return true;
    
    switch (formationId)
    {
        case FBDefs::FORMATION::F_4_4_2:
            do
            {
                CC_SAFE_DELETE(m_formation);
                m_formation = new CFBFormation442();
                m_formation->init(this);
                return true;
            } while (false);
            return false;
        case FBDefs::FORMATION::F_3_5_2:
            do
            {
                CC_SAFE_DELETE(m_formation);
                m_formation = new CFBFormation352();
                m_formation->init(this);
                return true;
            } while (false);
            return false;
        default:
            break;
    }
    
    return false;
}
