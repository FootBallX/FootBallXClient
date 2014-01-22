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

bool CFBTeam::init()
{
    do
    {
        // player 对象应该从哪里来？服务器？这里暂时创建一下。
        for (int i = 0; i < 20; ++i)
        {
            m_teamMembers.push_back(new CFBPlayer);
        }
        m_formation = new CFBFormation442();
        BREAK_IF_FAILED(m_formation->init());
        return true;
    } while (false);
    
    return false;
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
    
}



