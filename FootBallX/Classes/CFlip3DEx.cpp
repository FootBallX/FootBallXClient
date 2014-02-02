//
//  CFlip3DEx.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-28.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFlip3DEx.h"

CFlip3DYEx* CFlip3DYEx::create(float duration, float fromAngle, float toAngle)
{
    CFlip3DYEx *action = new CFlip3DYEx();
    
    if (action)
    {
        if (action->initWithDuration(duration))
        {
            action->m_from = fromAngle * M_PI / 180.f;
            action->m_to = toAngle * M_PI / 180.f;
            action->autorelease();
        }
        else
        {
            CC_SAFE_RELEASE_NULL(action);
        }
    }
    
    return action;
}

// Overrides
void CFlip3DYEx::update(float time)
{
//    float angle = (float)M_PI * time; // 180 degrees
    float angle = m_from + (m_from - m_to) * time;
    float mz = sinf( angle );
    angle = angle / 2.0f;     // x calculates degrees from 0 to 90
    float my = cosf(angle);
    
    Vertex3F    v0, v1, v, diff;
    
    v0 = getOriginalVertex(Point(1, 1));
    v1 = getOriginalVertex(Point(0, 0));
    
    float    y0 = v0.y;
    float    y1 = v1.y;
    float y;
    Point    a, b, c, d;
    
    if (y0 > y1)
    {
        // Normal Grid
        a = Point(0,0);
        b = Point(0,1);
        c = Point(1,0);
        d = Point(1,1);
        y = y0;
    }
    else
    {
        // Reversed Grid
        b = Point(0,0);
        a = Point(0,1);
        d = Point(1,0);
        c = Point(1,1);
        y = y1;
    }
    
    diff.y = y - y * my;
    diff.z = fabsf(floorf((y * mz) / 4.0f));
    
    // bottom-left
    v = getOriginalVertex(a);
//    v.y = diff.y;
    v.z += 0;//diff.z;
    setVertex(a, v);
    
    // upper-left
    v = getOriginalVertex(b);
    v.y -= 2 * diff.y;
    v.z -= 2* diff.z;
    setVertex(b, v);
    
    // bottom-right
    v = getOriginalVertex(c);
//    v.y = diff.y;
    v.z += 0;//diff.z;
    setVertex(c, v);
    
    // upper-right
    v = getOriginalVertex(d);
    v.y -= 2 * diff.y;
    v.z -= 2 * diff.z;
    setVertex(d, v);

}