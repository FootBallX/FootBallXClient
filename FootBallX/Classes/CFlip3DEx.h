//
//  CFlip3DEx.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-28.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFlip3DEx__
#define __FootBallX__CFlip3DEx__

#include "Common.h"

class CFlip3DYEx
: public FlipX3D
{
public:
    CFlip3DYEx() = default;
    virtual ~CFlip3DYEx() = default;
    
    static CFlip3DYEx* create(float duration, float fromAngle, float toAngle);
    
    // Overrides
    virtual void update(float time) override;
//	virtual CFlip3DYEx* clone() const override;
    
protected:
    float m_from = 0.f;
    float m_to = 0.f;
};

#endif /* defined(__FootBallX__CFlip3DEx__) */
