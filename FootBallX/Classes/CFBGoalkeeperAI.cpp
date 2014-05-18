//
//  CFBGoalkeeperAI.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBGoalkeeperAI.h"
#include "CFBMatch.h"
#include "CRandomManager.h"

bool CFBGoalkeeperAI::init(CFBFormation* formation, CFBPlayer* player, const cocos2d::Point& intPos, const cocos2d::Point& homePos, float radius, bool networkControl)
{
    do
    {
        BREAK_IF_FAILED(CFBPlayerAI::init(formation, player, intPos, homePos, radius, networkControl));
        m_player->m_isGoalKeeper = true;
        return true;
    } while (false);
    
    return false;
}


void CFBGoalkeeperAI::updateHomePosition()
{
    auto pitch = FBMATCH->getPitch();
    
    int pitchHeight = pitch->getPitchHeight();
    int halfPitchHeight = pitchHeight * 0.5f;
    auto& ballPos = FBMATCH->getBallPosition();
    
    float yRate = (ballPos.y - halfPitchHeight) / halfPitchHeight;
    float yOffset = yRate * .2;
    
    m_homePosition.y = m_origHomePosition.y + yOffset;
    m_homePosition.x = m_origHomePosition.x;
}



void CFBGoalkeeperAI::update(float dt)
{
    CFBPlayerAI::update(dt);
    
//    switch (m_state)
//    {
//        case FBDefs::AI_STATE::BACKHOME:
//            returnToHome(dt);
//            break;
//        case FBDefs::AI_STATE::WAIT:
//            updateWait(dt);
//            break;
//        default:
//            break;
//    }
}



void CFBGoalkeeperAI::thinkControlBall()
{
    int rNum = RANDOM_MGR->getRand() % 100;
    auto team = m_formation->getTeam();
    
    if (rNum < 50 && team->canShootDirectly(this->m_player))
    {
        // shoot
    }
    else
    {
        rNum = RANDOM_MGR->getRand() % 100;
        if (rNum < 0)
        {
            // dribber
        }
        else
        {
            // pass
            team->updateFieldStatusOnAttack();
            
            auto target = m_formation->getPassBallTarget();
            if (target)
            {
                FBMATCH->tryPassBall(m_player, target->getPlayer());
            }
        }
    }
}
