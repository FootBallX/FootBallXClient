//
//  CFBBall.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-27.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBBall__
#define __FootBallX__CFBBall__

#include "Common.h"

class CFBPlayer;
class CFBTeam;

class CFBBall
{
public:
    CFBBall() = default;
    virtual ~CFBBall() = default;
    
    void setBallPos(const cocos2d::Point& pos);
    const cocos2d::Point& getBallPos() const { return m_curPosition;};

protected:
    cocos2d::Point m_curPosition;
};

#endif /* defined(__FootBallX__CFBBall__) */
