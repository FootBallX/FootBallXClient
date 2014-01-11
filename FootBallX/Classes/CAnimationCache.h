//
//  CAnimationCache.h
//  GingyBattle
//
//  Created by Ming Zhao on 4/1/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#ifndef __GingyBattle__GBAnimationCache__
#define __GingyBattle__GBAnimationCache__

#include "Common.h"


class CAnimationCache
{
public:
    static CAnimationCache* getInstance();
    
    virtual void addAnimations(const char* pFileName);
    virtual Animate* getAnimateByAnimationName(const char* name);
    virtual cocos2d::Animation* getAnimationByName(const char* name);
    virtual Action* createActionByAnimationName(const char* name, bool repeated = true, CallFunc* callback = nullptr);
    virtual Action* createMoveToAction(float duration, Point pos, CallFunc* callback = nullptr);
    virtual Action* createPopAction(float duration, CallFunc* callback = nullptr);
    virtual Action* createVibrateAction(float duration, CallFunc* callback = nullptr);
    virtual Action* createMovePauseAction(float moveDuration, float pauseDuration, Point pausePosition, CallFunc* callback);
    virtual Action* createFadeToAction(float duration, GLubyte fadeTo, CallFunc* callback = nullptr);
    
protected:
    
private:
    CAnimationCache();
    ~CAnimationCache();
    
    static CAnimationCache* spInstance_;
    
};

#define ANIMATION_CACHE (CAnimationCache::getInstance())

#endif /* defined(__GingyBattle__GBAnimationCache__) */
