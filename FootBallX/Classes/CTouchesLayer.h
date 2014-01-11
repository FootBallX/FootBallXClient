//
//  CTouchesLayer.h
//  GingerBread
//
//  Created by Ming Zhao on 4/10/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#ifndef __GingerBread__CTouchesLayer__
#define __GingerBread__CTouchesLayer__

#include "Common.h"
#include "CBaseLayer.h"


class CTouchesLayer : public CBaseLayer
{
    // Gesture settings
    CC_SYNTHESIZE(float, m_swipeThreshold, SwipeThreshold);
    CC_SYNTHESIZE(float, m_swipeTime, SwipeTime);
    CC_SYNTHESIZE(float, m_tapThreshold, TapThreshold);
    CC_SYNTHESIZE(float, m_tapTime, TapTime);
    CC_SYNTHESIZE(float, m_longTapThreshold, LongTapThreshold);
    CC_SYNTHESIZE(float, m_longTapTime, LongTapTime);
    CC_SYNTHESIZE(float, m_doubleTapTime, DoubleTapTime);
    
public:
    enum GBTouchDirection
    {
        TOUCH_DIRECTION_LEFT,
        TOUCH_DIRECTION_RIGHT,
        TOUCH_DIRECTION_UP,
        TOUCH_DIRECTION_DOWN
    };
    
    enum GBGesture
    {
        GESTURE_TAP,
        GESTURE_LONGTAP,
        GESTURE_SWIPE
    };
    
    CTouchesLayer();
    ~CTouchesLayer();
    
    
    // Override of touches
    virtual void onTouchesBegan(const std::vector<Touch*>& touches, Event* event) override;
    virtual void onTouchesMoved(const std::vector<Touch*>& touches, Event* event) override;
    virtual void onTouchesEnded(const std::vector<Touch*>& touches, Event* event) override;
    

    // Override for subclsss
    virtual void touchesBegan(const std::vector<Touch*>& touches, Event* event);
    virtual void touchesMoved(const std::vector<Touch*>& touches, Event* event);
    virtual void touchesEnded(const std::vector<Touch*>& touches, Event* event);
    
    // Easy touch callbacks
    virtual void touchBegan(Point position);
    virtual void touchMoved(Point position);
    virtual void touchEnded(Point position);
    
    // Gesture Callbacks
    virtual void tapGesture(Point position);
    virtual void doubleTapGesture(Point position);
    virtual void longTapGesture(Point position);
    virtual void swipeGesture(GBTouchDirection direction, float distance, float speed);
    
protected:

private:
    
    void tapHandler(float caller);
    
    GBTouchDirection touchDirection_;
    // Touch positions
    Point gestureStartPoint_;
    Point gestureEndPoint_;
    Point actualPoint_;
    Point previousPoint_;

    bool touchHasMoved_;
    long touchStart_;
    bool swipeDone_;
    int  numberOfTaps_;
    
    long millisecondNow();
};

#endif /* defined(__GingerBread__CTouchesLayer__) */
