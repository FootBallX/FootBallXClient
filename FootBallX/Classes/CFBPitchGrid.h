//
//  CFBPitchGrid.h
//  FootBallX
//
//  Created by 马 俊 on 14-2-6.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBPitchGrid__
#define __FootBallX__CFBPitchGrid__

#include "Common.h"
#include "FBDefs.h"

class CFBPitchGrid
{
public:
    cocos2d::Point m_position;
    int m_score = 0;
    int m_index = -1;
    int m_shootAngleScore = 0;
    int m_shootDistanceScore = 0;
    
    float m_shootAngle = FLT_MAX;
    kmMat3 m_shootLineMat;
    
    cocos2d::Point m_transformedPos;
    cocos2d::Point m_transformedGoalPos;
#ifdef SHOW_GRID
    DrawNode* m_drawNode = nullptr;
#endif
protected:
    
};

#endif /* defined(__FootBallX__PitchGrid__) */
