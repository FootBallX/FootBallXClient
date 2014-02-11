//
//  FBDefs.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-25.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef FootBallX_FBDefs_h
#define FootBallX_FBDefs_h

#include "Common.h"

namespace FBDefs
{
    enum class SIDE : unsigned int
    {
        LEFT,
        RIGHT,
        NONE,
    };
    
    enum class FORMATION
    {
        F_4_4_2,
        F_3_5_2,
        NONE,
    };
    
    
    enum class TEAM_STATE
    {
        KICKOFF,
        ATTACKING,
        DEFENDING,
        NONE,
    };
    
    enum class AI_STATE
    {
        WAIT,
        BACKHOME,
        CONTROL,
        SUPPORT,
        CHASE,
        NONE,
    };
    
    bool isPointOnTheWay(const cocos2d::Point& p1, const cocos2d::Point& p2, const cocos2d::Point& p);
    
}


#endif
