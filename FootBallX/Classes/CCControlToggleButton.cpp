//
//  CCControlToggleButton.cpp
//  GingyBattle
//
//  Created by Ming Zhao on 4/24/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#include "CCControlToggleButton.h"

CCControlToggleButton::CCControlToggleButton()
: m_toggleValue(false)
, toggleOn_(nullptr)
, toggleOff_(nullptr)
{
    
}


CCControlToggleButton::~CCControlToggleButton()
{
    setToggleOn(nullptr);
    setToggleOff(nullptr);
}


CCControlToggleButton* CCControlToggleButton::createWithToggles(Scale9Sprite* toggleOn, Scale9Sprite* toggleOff)
{
    CCControlToggleButton *pRet = new CCControlToggleButton();

    if (pRet && pRet->initWithToggles(toggleOn, toggleOff))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}


bool CCControlToggleButton::initWithToggles(Scale9Sprite* toggleOn, Scale9Sprite* toggleOff)
{
    CCAssert(toggleOn && toggleOff, "Invalid Scale9Sprite.");
    
    do
    {
        CC_BREAK_IF(!initWithBackgroundSprite(toggleOn));
        setToggleOn(toggleOn);
        setToggleOff(toggleOff);
        m_toggleValue = true;
        
        return true;
    } while (false);
    
    return false;
}


bool CCControlToggleButton::getToggleValue()
{
    return m_toggleValue;
}


void CCControlToggleButton::setToggleValue(bool val)
{
    m_toggleValue = val;
    
    if (m_toggleValue)
    {
        setBackgroundSpriteForState(toggleOn_, Control::State::NORMAL);
    }
    else
    {
        setBackgroundSpriteForState(toggleOff_, Control::State::NORMAL);
    }
}


bool CCControlToggleButton::onTouchBegan(Touch* pTouch, Event *pEvent)
{    
    return ControlButton::onTouchBegan(pTouch, pEvent);
}


void CCControlToggleButton::onTouchMoved(Touch *pTouch, Event *pEvent)
{
    ControlButton::onTouchMoved(pTouch, pEvent);
}


void CCControlToggleButton::onTouchEnded(Touch *pTouch, Event *pEvent)
{
    if (isTouchInside(pTouch))
    {
//        m_toggleValue = !m_toggleValue;
//        if (m_toggleValue)
//        {
//            setBackgroundSpriteForState(toggleOn_, Control::State::NORMAL);
//        }
//        else
//        {
//            setBackgroundSpriteForState(toggleOff_, Control::State::NORMAL);
//        }
        setToggleValue(!m_toggleValue);
    }
    
    ControlButton::onTouchEnded(pTouch, pEvent);
}


void CCControlToggleButton::onTouchCancelled(Touch *pTouch, Event *pEvent)
{
    ControlButton::onTouchCancelled(pTouch, pEvent);
}

