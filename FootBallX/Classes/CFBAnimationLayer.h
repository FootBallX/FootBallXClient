//
//  CFBAnimationLayer.h
//  FootBallX
//
//  Created by 马 俊 on 14-3-16.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBAnimationLayer__
#define __FootBallX__CFBAnimationLayer__

#include "Common.h"

class CFBAnimationLayer
: public Layer
, public cocosbuilder::CCBSelectorResolver
{
public:
    CREATE_FUNC(CFBAnimationLayer);
    
    virtual cocos2d::SEL_MenuHandler onResolveCCBCCMenuItemSelector(cocos2d::Ref* pTarget, const char* pSelectorName) override;
    virtual cocos2d::SEL_CallFuncN onResolveCCBCCCallFuncSelector(cocos2d::Ref* pTarget, const char* pSelectorName) override;
    virtual cocos2d::extension::Control::Handler onResolveCCBCCControlSelector(cocos2d::Ref* pTarget, const char* pSelectorName) override;
    
    void setAnimationEndCallback(function<void()> cb);
protected:
    void onAnimationEnd(Node* node);
    
    function<void()> m_onAnimationEnd;
};


class CFBAnimationLayerLoader : public cocosbuilder::NodeLoader
{
public:
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_METHOD(CFBAnimationLayerLoader, loader);
protected:
    CCB_VIRTUAL_NEW_AUTORELEASE_CREATECCNODE_METHOD(CFBAnimationLayer);
};

#endif /* defined(__FootBallX__CFBAnimationLayer__) */
