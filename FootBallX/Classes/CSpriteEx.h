//
//  CSpriteEx.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-27.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CSpriteEx__
#define __FootBallX__CSpriteEx__

#include "Common.h"

class CSpriteEx
: public Sprite
{
public:
    CREATE_FUNC(CSpriteEx);
    
    virtual void prepareSpriteEx();
    //沿Y轴旋转degree角度
    virtual void rotateY(float degree);
    
    virtual void rotateX(float degree);
protected:
    //四个顶点数据原始值
    V3F_C4B_T2F_Quad m_sQuadOri;
};


class CSpriteExLoader : public cocosbuilder::SpriteLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CSpriteExLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CSpriteEx);
};


#endif /* defined(__FootBallX__CSpriteEx__) */
