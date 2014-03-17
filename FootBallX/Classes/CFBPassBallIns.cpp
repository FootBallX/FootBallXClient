//
//  CFBPassBallIns.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-14.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBPassBallIns.h"
#include "CFBFunctionsJS.h"
#include "CFBPlayer.h"

void CFBPassBallIns::start(function<CALLBACK_TYPE> callback)
{
    CC_ASSERT(m_players.size() >= 2);
    
    CFBInstruction::start(callback);
    auto player = m_players[0];
    auto& o1 = player->getPlayerCard();
    FB_FUNC_JS->startPassBall(o1);
    m_animationPlaying = true;
    m_step = 1;
    m_success = true;
}



void CFBPassBallIns::update(float dt)
{
    if (!m_animationPlaying)
    {
        auto count = m_players.size();
        if (m_success)
        {
            if (m_step < count - 1)
            {
                auto& o1 = m_players[0]->getPlayerCard();
                auto& o2 = m_players[m_step]->getPlayerCard();
                m_success = FB_FUNC_JS->tackleBall(o1, o2);     // TODO: or slideBall or interceptBall,But how to decide?
                m_animationPlaying = true;
                m_step++;
            }
            else if (m_step == (count -1))
            {
                auto player = m_players[m_step];
                auto& o1 = player->getPlayerCard();
                FB_FUNC_JS->recieveBall(o1);
                m_animationPlaying = true;
                m_step++;
                
                m_players[0]->loseBall();
                player->gainBall();
            }
            else
            {
                onInstructionEnd();
            }
        }
        else
        {
            if (m_step == count)
            {
                onInstructionEnd();
            }
            else
            {
                auto player = m_players[m_step - 1];
                auto& o1 = player->getPlayerCard();
                FB_FUNC_JS->recieveBall(o1);    // TODO: 这里可能要用个专门的抢到球的函数
                m_animationPlaying = true;
                m_step = (int)count;
                
                m_players[0]->loseBall();
                player->gainBall();
            }
        }
    }
}



void CFBPassBallIns::onAnimationEnd()
{
    m_animationPlaying = false;
}



void CFBPassBallIns::onInstructionEnd()
{
    CFBInstruction::onInstructionEnd();
    
    m_players.clear();
}
