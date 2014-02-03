//
//  CFBPitch.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-10.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBPitch__
#define __FootBallX__CFBPitch__

#include "Common.h"
#include "CFBTeam.h"
#include "FBDefs.h"

class CFBPitch
{
public:
    cocos2d::Point transformPersentage(const cocos2d::Point& pt, FBDefs::SIDE side);
    float transformPersentage(float p);
    
    void setPitchSize(int w, int h);
    int getPitchWidth() const { return m_width; }
    int getPitchHeight() const { return m_height; }
protected:
private:
    int m_width = 0;
    int m_height = 0;
    cocos2d::Point m_Goals[2];
};

#endif /* defined(__FootBallX__CFBPitch__) */
