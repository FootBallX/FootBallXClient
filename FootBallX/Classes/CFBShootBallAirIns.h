//
//  CFBShootBallAirIns.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-30.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBShootBallAirIns__
#define __FootBallX__CFBShootBallAirIns__

#include "CFBInstruction.h"
#include "FBDefs.h"

class CFBShootBallAirIns : public CFBInstruction
{
public:
    CFBShootBallAirIns() = default;
    virtual ~CFBShootBallAirIns() = default;
    
    virtual void update(float dt) override;
    virtual void start(function<CALLBACK_TYPE> callback) override;
    virtual void onAnimationEnd() override;
    
    virtual FBDefs::MATCH_FLOW_TYPE getInstructionType() override { return FBDefs::MATCH_FLOW_TYPE::SHOOT_AIR; }
protected:
    virtual void onInstructionEnd() override;
    
    int m_step = 0;
    bool m_animationPlaying = false;
    FBDefs::JS_RET_VAL m_ret = FBDefs::JS_RET_VAL::NONE;
    bool m_isAirBall = false;
};

#endif /* defined(__FootBallX__CFBShootBallAirIns__) */
