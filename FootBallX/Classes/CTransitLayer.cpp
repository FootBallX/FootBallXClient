//
//  CTransitLayer.cpp
//  GingyBattle
//
//  Created by Ming Zhao on 3/1/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#include "CTransitLayer.h"
#include "CAnimationCache.h"

#define TAG_LOADING_SPRITE      100

IMPLEMENT_SINGLETON(CTransitLayer);


CTransitLayer::CTransitLayer()
{
    
}


CTransitLayer::~CTransitLayer()
{
}


bool CTransitLayer::init()
{  
    if (!this->addLayers())
    {
        return false;
    }
    
    return true;
}


void CTransitLayer::onEnter()
{
    CBaseLayer::onEnter();
    
    this->runLoadingAnimation(true);
}


void CTransitLayer::onExit()
{
    CBaseLayer::onExit();
    
    Director::getInstance()->setNotificationNode(nullptr);
}


void CTransitLayer::unLoad()
{
    this->runAction(ANIMATION_CACHE->createFadeToAction(1.0f, 255, CallFunc::create(bind(&CTransitLayer::onExit, this))));
}


bool CTransitLayer::addLayers()
{
    // Color layer
    if (!Layer::init())
    {
        return false;
    }
    
    // Background
    Scale9Sprite* pBackground = CBaseLayer::createTile("background", _contentSize, Point(_contentSize.width / 2, _contentSize.height / 2));
    this->addChild(pBackground, -100);
    
    // Add loading aniamtion
    Sprite* loadingSprite = Sprite::createWithSpriteFrameName("loading0");
    loadingSprite->setPosition(Point(_contentSize.width / 2, _contentSize.height / 2));
    this->addChild(loadingSprite, 0, TAG_LOADING_SPRITE);
    
    return true;
}


void CTransitLayer::runLoadingAnimation(bool needCallback)
{
    Action* action = nullptr;
    if (needCallback)
    {
        action = ANIMATION_CACHE->createActionByAnimationName("Loading",
                                                              false,
                                                              CallFunc::create(bind(&CTransitLayer::loadingAnimationDidFinished, this)));

    }
    else
    {
        action = ANIMATION_CACHE->createActionByAnimationName("Loading", true);
    }
    
    Sprite* loadingSprite = static_cast<Sprite*>(getChildByTag(TAG_LOADING_SPRITE));
    CCAssert(loadingSprite, "Invalid loading sprite.");
    loadingSprite->runAction(action);
}


void CTransitLayer::loadingAnimationDidFinished()
{
    go(m_outLayer, false);
}


