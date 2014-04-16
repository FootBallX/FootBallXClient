//
//  FBDefs.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-2-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "FBDefs.h"
#include "Common.h"
#include "CFBPlayer.h"
#include "CFBPitchGrid.h"

#define WAY_WIDTH       10.f

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
    
    
    bool isPlayersOnTheWayToGoal(const vector<CFBPlayer*>& players, const CFBPitchGrid* grid)
    {        
        const kmMat3& mat = grid->m_shootLineMat;
        kmVec2 vP;
        
        const Point& vP1 = grid->m_transformedPos;
        const Point& vP2 = grid->m_transformedGoalPos;
        
        for (auto x : players)
        {
            if (x->m_isOnDuty && !x->m_isGoalKeeper)
            {
                const auto& p = x->getPosition();
                kmVec2Fill(&vP, p.x, p.y);
                kmVec2Transform(&vP, &vP, &mat);
                
                if (FLT_GE(vP.x, min(vP1.x, vP2.x)) && FLT_LE(vP.x, max(vP1.x, vP2.x)) && FLT_LE(abs(vP.y - vP1.y), WAY_WIDTH))
                {
                    return true;
                }
            }
        }
        
        return false;
    }
    
    
    
    bool computeGridShootAngleAndMat(const cocos2d::Point& goalPos, CFBPitchGrid* grid)
    {
        if (nullptr == grid)
        {
            return false;
        }
        
        Point& p1 = grid->m_position;
        const Point& p2 = goalPos;
        cocos2d::Point vec = p2 - p1;
        grid->m_shootAngle = vec.getAngle();

        kmMat3Rotation(&(grid->m_shootLineMat), -grid->m_shootAngle);
        
        kmVec2 v;
        kmVec2Fill(&v, goalPos.x, goalPos.y);
        kmVec2Transform(&v, &v, &grid->m_shootLineMat);
        grid->m_transformedGoalPos.setPoint(v.x, v.y);
        kmVec2Fill(&v, grid->m_position.x, grid->m_position.y);
        kmVec2Transform(&v, &v, &grid->m_shootLineMat);
        grid->m_transformedPos.setPoint(v.x, v.y);
        
        grid->m_shootAngle = CC_RADIANS_TO_DEGREES(grid->m_shootAngle);
        
        if (goalPos.x > grid->m_position.x)
        {
            grid->m_shootAngleScore = 180 - abs(grid->m_shootAngle);
        }
        else
        {
            grid->m_shootAngleScore = abs(grid->m_shootAngle);
        }
        
        grid->m_shootDistanceScore = goalPos.getDistance(grid->m_position);
        
        return true;
    }
}
