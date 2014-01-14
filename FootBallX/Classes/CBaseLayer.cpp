//
//  CBaseLayer.cpp
//  GingyBattle
//
//  Created by Ming Zhao on 3/1/13.
//  Copyright (c) 2013 Tencent. All rights reserved.
//

#include "CBaseLayer.h"
#include "Common.h"
#include "CTransitLayer.h"
#include "CCControlToggleButton.h"


CBaseLayer::CBaseLayer()
{
    
}


CBaseLayer::~CBaseLayer()
{
}


void CBaseLayer::onEnter()
{
    Layer::onEnter();
}


void CBaseLayer::onExit()
{
    Layer::onExit();
}


void CBaseLayer::onEnterTransitionDidFinish()
{
    Layer::onEnterTransitionDidFinish();
}


void CBaseLayer::unLoadTransitLayer()
{
//    Director* pDirector = Director::getInstance();
    
//    CTransitLayer* pLayer = dynamic_cast<CTransitLayer*>(pDirector->getNotificationNode());
//    if (pLayer)
//    {
//        pLayer->unLoad();
//    }
}


Scale9Sprite* CBaseLayer::createTile(const char* spriteFrameName, Size size, Point position)
{
    Scale9Sprite* tile = Scale9Sprite::createWithSpriteFrameName(spriteFrameName);
    if (size.equals(Size::ZERO))
    {
        Sprite* pSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
        if (pSprite)
        {
            size = pSprite->getContentSize();
        }
    }
    
    tile->setPreferredSize(size);
    tile->setAnchorPoint(Point(0.5f, 0.5f));
    tile->setPosition(position);
    return tile;
}


ControlButton* CBaseLayer::createImageButton(const char* spriteFrameName, Point position)
{
    CCAssert(spriteFrameName && strlen(spriteFrameName) > 0, "Invalid sprite frame name.");
    
    do
    {
        Sprite* pSprite = Sprite::createWithSpriteFrameName(spriteFrameName);
        CC_BREAK_IF(nullptr == pSprite);
        Size size = pSprite->getContentSize();
        
        ControlButton* pButton = ControlButton::create(Scale9Sprite::createWithSpriteFrameName(spriteFrameName));
        CC_BREAK_IF(nullptr == pButton);
        pButton->setPreferredSize(size);
        pButton->setPosition(position);
        
        return pButton;
    } while (false);
    
    return nullptr;
}


ControlButton* CBaseLayer::createImageButtonWithTitle(const char* title, const char* font, int fontSize,  const char* spriteFrameName, Point position)
{
    CCAssert(title && strlen(title) > 0, "Invalid title.");
    CCAssert(spriteFrameName && strlen(spriteFrameName) > 0, "Invalid sprite frame name.");
    
    do
    {
        LabelTTF* pLabel = LabelTTF::create(title, font, fontSize);
        CC_BREAK_IF(nullptr == pLabel);
        Scale9Sprite* pBackground = Scale9Sprite::createWithSpriteFrameName(spriteFrameName);
        CC_BREAK_IF(nullptr == pBackground);
        ControlButton* pButton = ControlButton::create(pLabel, pBackground);
        CC_BREAK_IF(nullptr == pButton);
        Size size = pLabel->getContentSize();
        Size backgroundSize = pBackground->getContentSize();
        size.width = 2 + size.width > backgroundSize.width ? size.width : backgroundSize.width;
        size.height = 2 + size.height > backgroundSize.height ? size.height : backgroundSize.height;
        pButton->setPreferredSize(size);
        pButton->setPosition(position);
        
        return pButton;
    } while (false);
    
    return nullptr;
}


ControlButton* CBaseLayer::createImageButtonWithTitle(const char* title, GameFonts font, const char* spriteFrameName, Point position)
{
    CCAssert(title && strlen(title) > 0, "Invalid title.");
    CCAssert(spriteFrameName && strlen(spriteFrameName) > 0, "Invalid sprite frame name.");

    do
    {
        // majun
//        LabelBMFont* pLabel = LabelBMFont::create(title,
//                                                      TEXTURE_CACHE->getFontFileName(font).c_str());
//        CC_BREAK_IF(nullptr == pLabel);
//        Scale9Sprite* pBackground = Scale9Sprite::createWithSpriteFrameName(spriteFrameName);
//        CC_BREAK_IF(nullptr == pBackground);
//        ControlButton* pButton = ControlButton::create(pLabel, pBackground);
//        CC_BREAK_IF(nullptr == pButton);
//        Size size = pLabel->getContentSize();
//        Size backgroundSize = pBackground->getContentSize();
//        size.width = 2 + size.width > backgroundSize.width ? size.width : backgroundSize.width;
//        size.height = 2 + size.height > backgroundSize.height ? size.height : backgroundSize.height;
//        pButton->setPreferredSize(size);
//        pButton->setPosition(position);
//        
//        return pButton;
    } while (false);
    
    return nullptr;
}


CCControlToggleButton* CBaseLayer::createToggleButton(const char* toggleOn, const char* toggleOff, Point position)
{
    CCAssert(toggleOn && strlen(toggleOn) > 0, "Invalid sprite frame name.");
    CCAssert(toggleOff && strlen(toggleOff) > 0, "Invalid sprite frame name.");
    
    do
    {
        Sprite* pSpriteOn = Sprite::createWithSpriteFrameName(toggleOn);
        CC_BREAK_IF(nullptr == pSpriteOn);
        Scale9Sprite* pScale9SpriteOn = Scale9Sprite::createWithSpriteFrameName(toggleOn);
        CC_BREAK_IF(nullptr == pScale9SpriteOn);
        pScale9SpriteOn->setPreferredSize(pSpriteOn->getContentSize());

        Sprite* pSpriteOff = Sprite::createWithSpriteFrameName(toggleOff);
        CC_BREAK_IF(nullptr == pSpriteOff);
        Scale9Sprite* pScale9SpriteOff = Scale9Sprite::createWithSpriteFrameName(toggleOff);
        CC_BREAK_IF(nullptr == pScale9SpriteOff);
        pScale9SpriteOff->setPreferredSize(pSpriteOff->getContentSize());
        
        CCControlToggleButton* pButton = CCControlToggleButton::createWithToggles(pScale9SpriteOn, pScale9SpriteOff);
        CC_BREAK_IF(nullptr == pButton);
        pButton->setPreferredSize(pSpriteOn->getContentSize());
        pButton->setPosition(position);
        
        return pButton;
    } while (false);
    
    return nullptr;
}


LabelBMFont* CBaseLayer::createLabelBMFont(const char* title, GameFonts font, TextHAlignment alignment, Point position)
{
    CCAssert(title, "Invalid title.");
    
    do {
        // majun
//        LabelBMFont* pLabel = LabelBMFont::create(title, TEXTURE_CACHE->getFontFileName(font).c_str(), kCCLabelAutomaticWidth, alignment, Point(0.5f, 0.5f));
//        CC_BREAK_IF(nullptr == pLabel);
//        
//        switch (alignment) {
//            case kCCTextAlignmentLeft:
//                pLabel->setAnchorPoint(Point(0.0f, 0.5f));
//                break;
//            case kCCTextAlignmentRight:
//                pLabel->setAnchorPoint(Point(1.0f, 0.5f));
//                break;
//            case kCCTextAlignmentCenter:
//                pLabel->setAnchorPoint(Point(0.5f, 0.5f));
//                break;
//            default:
//                break;
//        }
//        
//        pLabel->setPosition(position);
//        return pLabel;
    } while (false);

    return nullptr;
}


void CBaseLayer::go(LAYERS layer, bool needTransit)
{
    Director* pDirector = Director::getInstance();
    
    if (needTransit && pDirector->getNotificationNode())
    {
        return;
    }
    
    if (needTransit && layer != LAYERS_NONE)
    {        
        TRANSIT_LAYER->setOutLayer(layer);
        pDirector->setNotificationNode(TRANSIT_LAYER);
        TRANSIT_LAYER->onEnter();
    }
    else
    {
        Scene* pScene = this->wrap(layer);
        
        if (pScene)
        {
            if (pDirector->getRunningScene())
            {
                pDirector->replaceScene(pScene);
            }
            else
            {
                pDirector->runWithScene(pScene);
            }
        }
    }
}


Scene* CBaseLayer::wrap(LAYERS layer)
{
    CBaseLayer* pLayer = nullptr;
    
//    switch (layer) {
//        case LAYERS_NONE:
//            pLayer = this;
//            break;
//        case LAYERS_MAIN:
////            pLayer = MAIN_LAYER;
//            break;
//        case LAYERS_GAME_BATTLE_FIELD:
//            pLayer = BATTLE_FIELD_LAYER;
//            break;
//        case LAYERS_SETTINGS:
////            pLayer = SETTINGS_LAYER;
//            break;
//        case LAYERS_UPGRADE:
////            pLayer = UPGRADE_LAYER;
//        default:
//            break;
//    }
    
    Scene *pScene = Scene::create();
    pScene->addChild(pLayer);
    
    return pScene;
}



void CBaseLayer::addCloseButton()
{
    // add a "close" icon to exit the progress. it's an autorelease object
    MenuItemImage *pCloseItem = MenuItemImage::create("CloseNormal.png",
                                                      "CloseSelected.png",
                                                      nullptr,
                                                      bind(&CBaseLayer::menuCloseCallback, this, std::placeholders::_1)
                                                      );

    pCloseItem->setPosition(Point(Director::getInstance()->getWinSize().width - 20, 20));
    
    // create menu, it's an autorelease object
    Menu* pMenu = Menu::create(pCloseItem, nullptr);
    pMenu->setPosition( Point::ZERO );
    this->addChild(pMenu, 1);
}


void CBaseLayer::menuCloseCallback(Object* pSender)
{
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}



EditBox* CBaseLayer::createEdit(const char* spriteFrameName, Size size, Rect rect)
{
    SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
    CC_ASSERT(pFrame);
    Size sz = pFrame->getRect().size;
    if (rect.equals(Rect::ZERO))
    {
        rect = Rect(1, 1, sz.width-2, sz.height-2);
    }
    Scale9Sprite* s9s = Scale9Sprite::createWithSpriteFrameName(spriteFrameName, rect);
    if (size.equals(Size::ZERO))
    {
        size = sz;
    }
    return EditBox::create(size, s9s);
}
