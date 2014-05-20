//
//  CFBGoalkeeperAI.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBGoalkeeperAI__
#define __FootBallX__CFBGoalkeeperAI__

#include "CFBPlayerAI.h"

class CFBGoalkeeperAI
: public CFBPlayerAI
{
public:
    CFBGoalkeeperAI() = default;
    virtual ~CFBGoalkeeperAI() = default;
    
    virtual bool init(CFBTeam* team, CFBPlayer* player, const cocos2d::Point& homePos, float orbit) override;

    virtual void update(float dt) override;
    
protected:
    virtual void updateHomePosition() override;
    virtual void thinkControlBall() override;
};

#endif /* defined(__FootBallX__CFBGoalkeeperAI__) */
