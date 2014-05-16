//
//  CFBInstructionResult.h
//  FootBallX
//
//  Created by 马 俊 on 14-5-16.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBInstructionResult__
#define __FootBallX__CFBInstructionResult__

#include "Common.h"

class CFBInstructionResult
{
public:
    class InsStructure
    {
    public:
        class Animation{
        public:
            Animation(int a, float d)
            : aniId(a), delay(d)
            {}
            int aniId;
            float delay;
        };
        
        InsStructure(int s, int p, int i, int r)
        : side(s), playerNumber(p), ins(i), result(r)
        {}
        int side;
        int playerNumber;
        int ins;
        int result;
        vector<Animation> animations;
    };
    
    vector<InsStructure> instructions;
    int ballSide;
    int playerNumber;
    float ballPosX;
    float ballPosY;
};

#endif /* defined(__FootBallX__CFBInstructionResult__) */
