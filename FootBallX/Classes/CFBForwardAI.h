//
//  CFBForwardAI.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-12.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBForwardAI__
#define __FootBallX__CFBForwardAI__

#include "CFBPlayerAI.h"

class CFBForwardAI
: public CFBPlayerAI
{
public:
    CFBForwardAI() = default;
    virtual ~CFBForwardAI() = default;
    
    virtual void update(float dt) override;
    virtual bool init(CFBFormation* formation, CFBPlayer* player, const cocos2d::Point& intPos, const cocos2d::Point& homePos, float radius, bool networkControl) override;

protected:
    virtual void updateHomePosition() override;
    virtual void considerSupport() override;
    virtual void thinkControlBall() override;
    
    virtual void updateAIControlBall(float dt) override;
    // suport
    FBDefs::AI_STATE_SUPPORT m_supportState = FBDefs::AI_STATE_SUPPORT::FIND_POS;
    
    virtual void updateSupport(float dt);
};

#endif /* defined(__FootBallX__CFBForwardAI__) */
