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
#include "CFBMatch.h"
#include "CRandomManager.h"
#include "FBDefs.h"

void CFBPassBallIns::start(function<CALLBACK_TYPE> callback)
{
    CC_ASSERT(m_players.size() >= 2);
    
    CFBInstruction::start(callback);
    
    checkAirBall();
    
    auto player = m_players[0];
    auto& o1 = player->getPlayerCard();
    FB_FUNC_JS->startPassBall(o1, m_isAirBall);
    m_animationPlaying = true;
    m_step = 1;
    
    m_ret = FBDefs::JS_RET_VAL::FAIL;
}



void CFBPassBallIns::checkAirBall()
{
    auto player = m_players[m_players.size() - 1];
    auto pitch = FBMATCH->getPitch();
    auto side = player->m_ownerTeam->getSide();
    m_isAirBall = pitch->isInPenaltyArea(player->getPosition(), pitch->getOtherSide(side));
}



void CFBPassBallIns::update(float dt)
{
    if (!m_animationPlaying)
    {
        auto count = m_players.size();
        switch (m_ret)
        {
            case FBDefs::JS_RET_VAL::FAIL:
            case FBDefs::JS_RET_VAL::RANDOM_BALL:
            {
                if (m_step < count - 1)
                {
                    auto& o1 = m_players[0]->getPlayerCard();
                    auto& o2 = m_players[m_step]->getPlayerCard();
                    switch (m_players[m_step]->getInstruction())
                    {
                        case FBDefs::PLAYER_INS::BLOCK:
                            m_ret = FB_FUNC_JS->blockBall(o1, o2);
                            break;
                        case FBDefs::PLAYER_INS::INTERCEPT:
                            m_ret = FB_FUNC_JS->interceptBall(o1, o2);
                            break;
                        case FBDefs::PLAYER_INS::TAKCLE:
                            m_ret = FB_FUNC_JS->tackleBall(o1, o2);
                            break;
                        default:
                            CC_ASSERT(false);
                            break;
                    }
                    
                    m_animationPlaying = true;
                    m_step++;
                }
                else if (m_step == (count -1))
                {
                    auto player = m_players[m_step];
                    auto& o1 = player->getPlayerCard();
                    FB_FUNC_JS->receiveBall(o1);
                    m_animationPlaying = true;
                    m_step++;
                    
                    m_players[0]->loseBall();
                    player->gainBall();
                }
                else
                {
                    onInstructionEnd();
                }
                
                break;
            }
                
            case FBDefs::JS_RET_VAL::SUCCESS:
            {
                if (m_step == count)
                {
                    onInstructionEnd();
                }
                else
                {
                    auto player = m_players[m_step - 1];
                    auto& o1 = player->getPlayerCard();
                    FB_FUNC_JS->receiveBall(o1);    // TODO: 这里可能要用个专门的抢到球的函数
                    m_animationPlaying = true;
                    m_step = (int)count;
                    
                    m_players[0]->loseBall();
                    player->gainBall();
                }
                
                break;
            }
            default:
                break;
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
