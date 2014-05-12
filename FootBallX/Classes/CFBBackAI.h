//
//  CFBBackAI.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBBackAI__
#define __FootBallX__CFBBackAI__

#include "CFBPlayerAI.h"

class CFBBackAI
: public CFBPlayerAI
{
public:
    CFBBackAI() = default;
    virtual ~CFBBackAI() = default;
    
    virtual void update(float dt) override;
    
protected:
    virtual void initPlayerStates(const cocos2d::Point& pt, bool networkControl) override;
    virtual void considerSupport() override;
    
    virtual void updateHomePosition() override;
    virtual void thinkControlBall() override;
    
    virtual void updateAIControlBall(float dt) override;
};

#endif /* defined(__FootBallX__CFBBackAI__) */
