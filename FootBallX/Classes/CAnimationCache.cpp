//
//  CAnimationCache.cpp
//  GingyBattle
//
//  Created by Ming Zhao on 4/1/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#include "CAnimationCache.h"
#include <stdio.h>

#define ANIMATION_TYPE                      "Type"
#define ANIMATION_FRAME_NAME                "FrameName"
#define ANIMATION_FRAME_NUM                 "FrameNum"
#define ANIMATION_DELAY_PER_UNIT            "DelayPerUnit"
#define ANIMATION_RESTORE_ORIGINAL_FRAME    "RestoreOriginalFrame"
#define ANIMATION_LOOP                      "Loop"

CAnimationCache* CAnimationCache::spInstance_ = nullptr;


CAnimationCache::CAnimationCache()
{
    
}


CAnimationCache::~CAnimationCache()
{
    
}


CAnimationCache* CAnimationCache::getInstance()
{
    if (!spInstance_)
    {
        spInstance_ = new CAnimationCache();
    }
    
    return spInstance_;
}


Animate* CAnimationCache::getAnimateByAnimationName(const char* name)
{
    do {
        CC_BREAK_IF(nullptr == name || strlen(name) <= 0);
        AnimationCache* animationCache = AnimationCache::getInstance();
        cocos2d::Animation* animation = animationCache->getAnimation(name);
        CC_BREAK_IF(nullptr == animation);
        Animate* animate = cocos2d::Animate::create(animation);

        return animate;
    } while (false);

    return nullptr;
}


cocos2d::Animation* CAnimationCache::getAnimationByName(const char* name)
{
    do {
        CC_BREAK_IF(nullptr == name || strlen(name) <= 0);
        AnimationCache* animationCache = AnimationCache::getInstance();
        cocos2d::Animation* animation = animationCache->getAnimation(name);
        
        return animation;
    } while (false);
    
    return nullptr;
}


Action* CAnimationCache::createActionByAnimationName(const char* name, bool repeated, CallFunc* callback)
{
    do
    {
        Animate* animate = getAnimateByAnimationName(name);
        CC_BREAK_IF(nullptr == animate);
        
        if (repeated)
        {
            return CCRepeatForever::create(animate);
        }
        else
        {
            if (callback)
            {
                return CCSequence::create(animate, callback, nullptr);
            }
            else
            {
                return CCSequence::create(animate, nullptr);
            }
        }
    } while (false);
    
    return nullptr;
}


Action* CAnimationCache::createMoveToAction(float duration, Point pos, CallFunc* callback)
{
    if (callback)
    {
        return CCSequence::create(CCMoveTo::create(duration, pos), callback, nullptr);
    }
    else
    {
        return CCSequence::create(CCMoveTo::create(duration, pos), nullptr);
    }
}


Action* CAnimationCache::createPopAction(float duration, CallFunc* callback)
{
    ActionInterval* scale = ScaleTo::create(0.0f, 0.2f);
    ActionInterval* scaleBack = CCEaseElasticOut::create(ScaleTo::create(duration, 1.0f));

    if (callback)
    {
        return CCSequence::create(scale, scaleBack, callback, nullptr);
    }
    else
    {
        return CCSequence::create(scale, scaleBack, nullptr);
    }
}



Action* CAnimationCache::createVibrateAction(float duration, CallFunc* callback)
{
//    FiniteTimeAction* scale1 = GBScale::create(1.05f);
//    FiniteTimeAction* vibrate = GBVibrate::create(duration);
//    FiniteTimeAction* scale2 = GBScale::create(1.f);
//    FiniteTimeAction* place = CCPlace::create(Point(0.f, 0.f));
//  
//    if (callback)
//    {
//        return CCSequence::create(scale1, vibrate, scale2, place, callback, nullptr);
//    }
//    else
//    {
//        return CCSequence::create(scale1, vibrate, scale2, place, nullptr);
//    }
    
    return nullptr;
}



Action* CAnimationCache::createMovePauseAction(float moveDuration, float pauseDuration, Point pausePosition,  CallFunc* callback)
{
    do {
        FiniteTimeAction* move = dynamic_cast<FiniteTimeAction*>(createMoveToAction(moveDuration, pausePosition));
        CC_BREAK_IF(nullptr == move);
        FiniteTimeAction* pause = CCDelayTime::create(pauseDuration);
        CC_BREAK_IF(nullptr == pause);
        
        if (callback)
        {
            return CCSequence::create(move, pause, callback, nullptr);
        }
        else
        {
            return CCSequence::create(move, pause, nullptr);
        }
        
    } while (false);
    return nullptr;
}



Action* CAnimationCache::createFadeToAction(float duration, GLubyte fadeTo, CallFunc* callback)
{
    do {
        FiniteTimeAction* fade = FadeTo::create(duration, fadeTo);
        CC_BREAK_IF(nullptr == fade);
        if (callback)
        {
            return CCSequence::create(fade, callback, nullptr);
        }
        else
        {
            return fade;
        }
        
    } while (false);
    
    return nullptr;
}
