//
//  CFBShootBallAirIns.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-30.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBShootBallAirIns.h"
#include "CFBFunctionsJS.h"
#include "CFBPlayer.h"
#include "CFBMatch.h"
#include "CRandomManager.h"

void CFBShootBallAirIns::start(function<CALLBACK_TYPE> callback)
{
    CC_ASSERT(m_players.size() >= 2);
    
    CFBInstruction::start(callback);
    
    auto player = m_players[0];
    auto& o1 = player->getPlayerCard();
    FB_FUNC_JS->startPassBall(o1, m_isAirBall);
    m_animationPlaying = true;
    m_step = 1;
    m_ret = FBDefs::JS_RET_VAL::SUCCESS;
    
}



void CFBShootBallAirIns::update(float dt)
{
    if (!m_animationPlaying)
    {
        auto count = m_players.size();
    }
}



void CFBShootBallAirIns::onAnimationEnd()
{
    m_animationPlaying = false;
}



void CFBShootBallAirIns::onInstructionEnd()
{
    CFBInstruction::onInstructionEnd();
    
    m_players.clear();
}