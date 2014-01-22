//
//  CFBMatch.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-21.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBMatch.h"
#include "CFBPitch.h"

IMPLEMENT_SINGLETON(CFBMatch);

CFBMatch::CFBMatch()
: m_pitch(nullptr)
, m_redTeam(nullptr)
, m_blackTeam(nullptr)
{
    
}



CFBMatch::~CFBMatch()
{
    CC_SAFE_DELETE(m_pitch);
}



bool CFBMatch::init()
{
    do
    {
        m_pitch = new CFBPitch;
        return true;
    } while (false);
    
    return false;
}

bool CFBMatch::startMatch(bool redTeamKickoff)
{
    do
    {
        m_redTeam->onStartMatch();
        m_blackTeam->onStartMatch();
        
        redTeamKickoff ? m_redTeam->kickOff() : m_blackTeam->kickOff();
        return true;
    } while (false);
    
    return false;
}