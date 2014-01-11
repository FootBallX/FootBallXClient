//
//  CCControlToggleButton.h
//  GingyBattle
//
//  Created by Ming Zhao on 4/24/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#ifndef __GingyBattle__CCControlToggleButton__
#define __GingyBattle__CCControlToggleButton__

#include "Common.h"

class CCControlToggleButton : public ControlButton
{
public:
    CCControlToggleButton();
    virtual ~CCControlToggleButton();
    
    static CCControlToggleButton* createWithToggles(Scale9Sprite* toggleOn, Scale9Sprite* toggleOff);
    virtual bool initWithToggles(Scale9Sprite* toggleOn, Scale9Sprite* toggleOff);

    //events
    virtual bool onTouchBegan(Touch *pTouch, Event *pEvent);
    virtual void onTouchMoved(Touch *pTouch, Event *pEvent);
    virtual void onTouchEnded(Touch *pTouch, Event *pEvent);
    virtual void onTouchCancelled(Touch *pTouch, Event *pEvent);
    
//    CC_SYNTHESIZE(bool, m_toggleValue, ToggleValue);
    CC_PROPERTY(bool, m_toggleValue, ToggleValue);
    CC_SYNTHESIZE_RETAIN(Scale9Sprite*, toggleOn_, ToggleOn);
    CC_SYNTHESIZE_RETAIN(Scale9Sprite*, toggleOff_, ToggleOff);
    
protected:
private:
};

#endif /* defined(__GingyBattle__CCControlToggleButton__) */
