//
//  CTouchesLayer.cpp
//  GingerBread
//
//  Created by Ming Zhao on 4/10/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#include "CTouchesLayer.h"

CTouchesLayer::CTouchesLayer()
: m_swipeThreshold(10)
, m_swipeTime(500)
, m_tapThreshold(10)
, m_tapTime(10)
, m_longTapThreshold(10)
, m_longTapTime(1000)
, m_doubleTapTime(10)
, numberOfTaps_(0)
, touchHasMoved_(false)
, touchStart_(0)
, swipeDone_(false)
{

}


CTouchesLayer::~CTouchesLayer()
{
    
}


long CTouchesLayer::millisecondNow()
{
    struct timeval now;
    gettimeofday(&now, nullptr);
    return (now.tv_sec * 1000 + now.tv_usec / 1000);
}


// Override of touches
void CTouchesLayer::onTouchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    this->touchHasMoved_ = false;
    this->touchStart_ = this->millisecondNow();
    
    Touch* fingerOne = touches[0];
    Point  pointOne = Director::getInstance()->convertToUI(fingerOne->getLocationInView());
    
    Point location = this->convertToNodeSpace(pointOne);
    this->gestureStartPoint_= location; //touch locationInView:touch->view);
    this->previousPoint_ = location;
    
    swipeDone_ = false;
    
    // Passthrough
    this->touchBegan(location);
    this->touchesBegan(touches, event);
    
}


void CTouchesLayer::onTouchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    long endTime = this->millisecondNow();
    long deltaTime = endTime - this->touchStart_;
    
    this->touchHasMoved_ = true;

    Touch* fingerOne = touches[0];
    Point pointOne = Director::getInstance()->convertToUI(fingerOne->getLocationInView());
    
    Point location = this->convertToNodeSpace(pointOne);
    this->actualPoint_ = location;
    
    if (!swipeDone_)
    {
        swipeDone_ = true;
        
        // Calculate the distance
        float deltaX = gestureStartPoint_.x - actualPoint_.x;
        float deltaY = gestureStartPoint_.y - actualPoint_.y;
        
        Size screenSize = Director::getInstance()->getWinSize();
        
        float horSwipeDistancePercentage = fabs((deltaX / screenSize.width) * 100);
        float verSwipeDistancePercentage = fabs((deltaY / screenSize.height) * 100);
        

        // Calculate the direction
        // First horizontal or vertical
        if (fabs(deltaX) > fabs(deltaY) && horSwipeDistancePercentage > this->m_swipeThreshold && deltaTime < this->m_swipeTime) // horizontal
        {
            if (deltaX < 0)
            {
                this->touchDirection_ = TOUCH_DIRECTION_RIGHT;
            }
            
            if (deltaX > 0)
            {
                this->touchDirection_ = TOUCH_DIRECTION_LEFT;
            }
            
            float speed = fabs(deltaX) / deltaTime;
            this->swipeGesture(this->touchDirection_, fabs(deltaX), speed);
        }
        else if (fabs(deltaX) < fabs(deltaY) &&
                 verSwipeDistancePercentage > this->m_swipeThreshold &&
                 deltaTime < this->m_swipeTime)  // Vertical
        {
            if (deltaY > 0)
            {
                this->touchDirection_ = TOUCH_DIRECTION_DOWN;
            }
            
            if (deltaY < 0)
            {
                this->touchDirection_ = TOUCH_DIRECTION_UP;
            }
            
            float speed = fabs(deltaY) / deltaTime;
            this->swipeGesture(this->touchDirection_, fabs(deltaY), speed);
        }
        else
        {
            swipeDone_ = false;
        }
    }
    
//    CCLog("Position:%f,%f",location.x,location.y);
    // Passthrough
    this->touchMoved(location);
    this->touchesMoved(touches, event);
}


void CTouchesLayer::onTouchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    long endTime = this->millisecondNow();
    long deltaTime = endTime - this->touchStart_;
//    CCLog("Deltatime %ld",deltaTime);
    
    Touch* fingerOne = touches[0];
    Point pointOne = Director::getInstance()->convertToUI(fingerOne->getLocationInView());
    
    Point location = this->convertToNodeSpace(pointOne);
    this->gestureEndPoint_ = location;//touch locationInView:touch->view);
    
    if (deltaTime >= this->m_longTapTime)// No movement, tap detected
    {
        this->longTapGesture(location);
    }
    else
    {
        if (this->numberOfTaps_ == 0)
        {
            this->touchStart_ = this->millisecondNow();
            this->schedule(schedule_selector(CTouchesLayer::tapHandler),
                           (this->m_doubleTapTime / 1000), 1, 0);
        }
        this->numberOfTaps_++;
    }
    
    swipeDone_ = false;
    
    touchEnded(location);
    touchesEnded(touches, event);
}



void CTouchesLayer::tapHandler(float caller)
{
    this->unschedule(schedule_selector(CTouchesLayer::tapHandler));
    
    if (this->numberOfTaps_ == 1 && !this->touchHasMoved_) // singletap
    {
        this->numberOfTaps_ = 0;
        this->tapGesture(this->gestureEndPoint_);
    }
    else if (this->numberOfTaps_ == 2) // double tap
    {
        this->numberOfTaps_ = 0;
        this->doubleTapGesture(this->gestureEndPoint_);
    }
    else
    {
        this->numberOfTaps_ = 0;
    }
}



void CTouchesLayer::touchBegan(Point position)
{
}


void CTouchesLayer::touchMoved(Point position)
{
}


void CTouchesLayer::touchEnded(Point position)
{
}


void CTouchesLayer::tapGesture(Point position)
{
}


void CTouchesLayer::doubleTapGesture(Point position)
{
}


void CTouchesLayer::longTapGesture(Point position)
{
}



void CTouchesLayer::touchesBegan(const std::vector<Touch*>& touches, Event* event)
{
    
}



void CTouchesLayer::touchesMoved(const std::vector<Touch*>& touches, Event* event)
{
    
}



void CTouchesLayer::touchesEnded(const std::vector<Touch*>& touches, Event* event)
{
    
}




void CTouchesLayer::swipeGesture(GBTouchDirection direction, float distance, float speed)
{
}
