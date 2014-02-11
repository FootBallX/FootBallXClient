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
    cocos2d::Point m_coordinate;
    int m_score = 0;
    int m_index = -1;
    
#ifdef SHOW_GRID
    DrawNode* m_drawNode = nullptr;
#endif
protected:
    
};

#endif /* defined(__FootBallX__PitchGrid__) */
