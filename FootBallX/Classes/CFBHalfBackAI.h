//
//  CFBHalfBackAI.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBHalfBackAI__
#define __FootBallX__CFBHalfBackAI__

#include "CFBPlayerAI.h"

class CFBHalfBackAI
: public CFBPlayerAI
{
public:
    CFBHalfBackAI() = default;
    virtual ~CFBHalfBackAI() = default;
    
    virtual void update(float dt) override;
    virtual bool init(CFBTeam* team, CFBPlayer* player, const cocos2d::Point& homePos, float orbit) override;

protected:
    virtual void considerSupport() override;
    
    virtual void updateHomePosition() override;
    virtual void thinkControlBall() override;
    
    virtual void updateAIControlBall(float dt) override;
};
#endif /* defined(__FootBallX__CFBHalfBackAI__) */
