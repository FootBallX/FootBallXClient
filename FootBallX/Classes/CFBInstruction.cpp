//
//  CFBInstruction.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-14.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBInstruction.h"
#include "CFBPassBallIns.h"
#include "CFBShootBallAirIns.h"
#include "CFBShootBallGroundIns.h"

void CFBInstruction::addPlayer(CFBPlayer* player)
{
    m_players.push_back(player);
}



void CFBInstruction::start(function<CALLBACK_TYPE> callback)
{
    m_callback = callback;
}



void CFBInstruction::onInstructionEnd()
{
    if (m_callback)
    {
        m_callback();
    }
}


#pragma mark - factory

IMPLEMENT_SINGLETON(CFBInstructionFactory);

CFBInstructionFactory::~CFBInstructionFactory()
{
    CC_SAFE_DELETE(m_passBallIns);
}


CFBPassBallIns* CFBInstructionFactory::getPassBallIns()
{
    if (nullptr == m_passBallIns)
    {
        m_passBallIns = new CFBPassBallIns();
    }
    
    return m_passBallIns;
}



CFBShootBallAirIns* CFBInstructionFactory::getShootBallAirIns()
{
    if (nullptr == m_shootBallAirIns)
    {
        m_shootBallAirIns = new CFBShootBallAirIns();
    }
    
    return m_shootBallAirIns;
}



CFBShootBallGroundIns* CFBInstructionFactory::getShootBallGroundIns()
{
    if (nullptr == m_shootBallGroundIns)
    {
        m_shootBallGroundIns = new CFBShootBallGroundIns();
    }
    
    return m_shootBallGroundIns;
}

