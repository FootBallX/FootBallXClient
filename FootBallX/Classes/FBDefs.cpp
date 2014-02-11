//
//  FBDefs.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-2-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "FBDefs.h"

#define WAY_WIDTH       (10.f * cocosbuilder::CCBReader::getResolutionScale())

namespace FBDefs
{
    bool isPointOnTheWay(const cocos2d::Point& p1, const cocos2d::Point& p2, const cocos2d::Point& p)
    {
        cocos2d::Rect bx(min(p1.x, p2.x), min(p1.y, p2.y), abs(p1.x - p2.x), abs(p1.y - p2.y));
        if (!bx.containsPoint(p))
        {
            return false;
        }
        
        cocos2d::Point vec = p2 - p1;
        const cocos2d::Rect rc;
        float angle = vec.getAngle();
        
        kmMat3 mat;
        kmMat3Rotation(&mat, -angle);
        kmVec2 vP1, vP2, vP;
        kmVec2Fill(&vP1, p1.x, p1.y);
        kmVec2Fill(&vP2, p2.x, p2.y);
        kmVec2Fill(&vP, p.x, p.y);
        kmVec2Transform(&vP1, &vP1, &mat);
        kmVec2Transform(&vP2, &vP2, &mat);
        kmVec2Transform(&vP, &vP, &mat);
        
        if (vP.x < min(vP1.x, vP2.x) || vP.x > max(vP1.x, vP2.x))
        {
            return false;
        }
        
        if (abs(vP.y - vP1.y) > WAY_WIDTH)
        {
            return false;
        }
        
        return true;
    }
}