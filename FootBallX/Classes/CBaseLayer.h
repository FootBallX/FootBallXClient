//
//  CBaseLayer.h
//  GingyBattle
//
//  Created by Ming Zhao on 3/1/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#ifndef __GingyBattle__GBBaseLayer__
#define __GingyBattle__GBBaseLayer__

#include "Common.h"

class CCControlToggleButton;


class CBaseLayer : public Layer
{
public:
    CBaseLayer();
    ~CBaseLayer();
    
    static Scale9Sprite* createTile(const char* spriteFrameName, Size size = Size::ZERO, Point position = Point::ZERO);
    static ControlButton* createImageButton(const char* spriteFrameName, Point position = Point::ZERO);
    static ControlButton* createImageButtonWithTitle(const char* title, GameFonts font, const char* spriteFrameName, Point position = Point::ZERO);
    static ControlButton* createImageButtonWithTitle(const char* title, const char* font, int fontSize, const char* spriteFrameName, Point position = Point::ZERO);
    static CCControlToggleButton* createToggleButton(const char* toggleOn, const char* toggleOff, Point position = Point::ZERO);
    static LabelBMFont* createLabelBMFont(const char* title, GameFonts font, TextHAlignment alignment = TextHAlignment::CENTER, Point position = Point::ZERO);
    static EditBox* createEdit(const char* spriteFrameName, Size size = Size::ZERO, Rect rect = Rect::ZERO);

    virtual void onEnter();
    virtual void onExit();
    virtual void onEnterTransitionDidFinish();
    virtual void unLoadTransitLayer();
    
    void go(LAYERS layer, bool needTransit = false);
    
    CC_SYNTHESIZE_RETAIN(Array*, m_popTargets, PopTargets);
    
protected:
    virtual Scene* wrap(LAYERS layer);
    virtual void addPopTarget(Node* pNode);
    virtual void addCloseButton();
    virtual void menuCloseCallback(Object* pSender);
    
private:
};

#endif /* defined(__GingyBattle__GBBaseLayer__) */
