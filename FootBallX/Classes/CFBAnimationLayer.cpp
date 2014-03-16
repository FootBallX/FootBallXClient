//
//  CFBAnimationLayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-3-16.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBAnimationLayer.h"

static class CFBAnimationLayerRegister
{
public:
    CFBAnimationLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "AnimationLayer", CFBAnimationLayerLoader::loader());
    }
} __reg;




SEL_MenuHandler CFBAnimationLayer::onResolveCCBCCMenuItemSelector(Object * pTarget, const char* pSelectorName)
{
    return nullptr;
}



//函数定义类型为：void pressTitle(Object *pSender);
Control::Handler CFBAnimationLayer::onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName)
{
    return nullptr;
}



cocos2d::SEL_CallFuncN CFBAnimationLayer::onResolveCCBCCCallFuncSelector(cocos2d::Object * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CALLFUNC_GLUE(this, "AnimationEnd", CFBAnimationLayer::onAnimationEnd);
    return nullptr;
}



void CFBAnimationLayer::setAnimationEndCallback(function<void()> cb)
{
    m_onAnimationEnd = cb;
}



void CFBAnimationLayer::onAnimationEnd(Node* node)
{
    if (m_onAnimationEnd)
    {
        m_onAnimationEnd();
    }
}
