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
    auto& o1 = m_players[0]->getPlayerCard();
    FB_FUNC_JS->startPassBall(o1);
    m_animationPlaying = true;
    m_step++;

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
                m_success = FB_FUNC_JS->tackleBall(o1, o2);
                m_animationPlaying = true;
                m_step++;
            }
            else if (m_step == (count -1))
            {
                auto& o1 = m_players[m_step]->getPlayerCard();
                FB_FUNC_JS->recieveBall(o1);
                m_animationPlaying = true;
                m_step++;
            }
            else
            {
                CallCallback();
            }
        }
        else
        {
            if (m_step == count)
            {
                CallCallback();
            }
            else
            {
                auto& o1 = m_players[m_step-1]->getPlayerCard();
                FB_FUNC_JS->recieveBall(o1);    // TODO: 这里可能要用个专门的抢到球的函数
                m_animationPlaying = true;
                m_step = (int)count;
            }
        }
    }
}



void CFBPassBallIns::onAnimationEnd()
{
    m_animationPlaying = false;
}
