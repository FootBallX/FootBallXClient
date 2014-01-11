//
//  CMessageBoxLayer.cpp
//  GameX
//
//  Created by 马 俊 on 13-11-21.
//
//

#include "CMessageBoxLayer.h"


CMessageBoxLayer::CMessageBoxLayer()
{
}



CMessageBoxLayer::~CMessageBoxLayer()
{
}


bool CMessageBoxLayer::init()
{
    do
    {
        Size sz = Director::getInstance()->getWinSize();
        
        setTouchMode(Touch::DispatchMode::ONE_BY_ONE);
        setSwallowsTouches(true);
        setContentSize(sz);
        
        SpriteFrame *pFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(BKG_FILENAME);
        CC_ASSERT(pFrame);
        Size insetSz = pFrame->getRect().size;
        Rect insetRect = {INSET, INSET, insetSz.width - INSET * 2, insetSz.height - INSET * 2};
        Scale9Sprite* background = Scale9Sprite::createWithSpriteFrame(pFrame, insetRect);
        
        background->setPosition(Point(sz) * 0.5f);
        addChild(background, Z_BKG, TAG_BKG);
        
        return true;
    } while (false);
    
    return false;
}


void CMessageBoxLayer::onButtonClicked(Object* sender, Control::EventType type)
{
    ControlButton* btn = static_cast<ControlButton*>(sender);
    if (m_callback)
    {
        if (m_callback(btn->getTag()))
        {
            closeModal();
        }
    }
    else
    {
        closeModal();
    }
}


bool CMessageBoxLayer::addButton(const char* t, BUTTON_TYPE btnType)
{
    do
    {
        BREAK_IF(m_buttonMask & btnType);
        
        Size sz = Director::getInstance()->getWinSize();
        ControlButton* btn = createImageButtonWithTitle(t, FONT_NAME, FONT_SIZE, "huds/hp_bar_fg0.png");
        CC_ASSERT(btn);
        btn->addTargetWithActionForControlEvents(this,
                                                 cccontrol_selector(CMessageBoxLayer::onButtonClicked),
                                                 Control::EventType::TOUCH_UP_INSIDE);

        addChild(btn, Z_BUTTON, btnType);
        
        m_buttons.push_back(btn);
    
        m_buttonMask |= btnType;

        return true;
    } while (false);
    
    return false;
}


void CMessageBoxLayer::setMsg(const char* msg)
{
    removeChildByTag(TAG_MSG);
    LabelTTF* msg_label = LabelTTF::create(msg, FONT_NAME, FONT_SIZE);
    addChild(msg_label, Z_MSG, TAG_MSG);

}



void CMessageBoxLayer::setTitle(const char* title)
{
    removeChildByTag(TAG_TITLE);
    LabelTTF* title_label = LabelTTF::create(title, FONT_NAME, FONT_SIZE);
    addChild(title_label, Z_TITLE, TAG_TITLE);
}



void CMessageBoxLayer::setCallback(function<bool(int)> f)
{
    m_callback = f;
}



void CMessageBoxLayer::doModal()
{
    setTouchEnabled(true);

    adjustBoxSize();
    
    Scene* pCurrentScene = Director::getInstance()->getRunningScene();
    CC_ASSERT(pCurrentScene);
    pCurrentScene->addChild(this, INT_MAX, TAG_THIS);
}



void CMessageBoxLayer::closeModal()
{
    setTouchEnabled(false);
    
    Scene* pCurrentScene = Director::getInstance()->getRunningScene();
    CC_ASSERT(pCurrentScene);
    pCurrentScene->removeChildByTag(TAG_THIS);
}



void CMessageBoxLayer::adjustBoxSize()
{
    Size sz = Director::getInstance()->getWinSize();
    Node* msg = getChildByTag(TAG_MSG);
    // calc msg label size
    if (msg)
    {
        Size labSize = msg->getContentSize();
        m_boxSize.width = max(m_boxSize.width, labSize.width + INSET * 2);
        m_boxSize.height += labSize.height;
        m_boxSize.height += V_GAP;
    }

    // calc title label size
    
    Node* title = getChildByTag(TAG_TITLE);
    if (title)
    {
        Size labSize = title->getContentSize();
        m_boxSize.width = max(m_boxSize.width, labSize.width + INSET * 2);
        m_boxSize.height += labSize.height;
        m_boxSize.height += V_GAP;
    }
    
    // calc button size
    {
        float maxWidth = 0.f;
        float maxHeight = 0.f;
        
        for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it)
        {
            Size cz = (*it)->getContentSize();
            if (cz.width > maxWidth)
            {
                maxWidth = cz.width;
            }
            if (cz.height > maxHeight)
            {
                maxHeight = cz.height;
            }
        }
        
        m_boxSize.width = max(m_boxSize.width, (maxWidth + H_GAP) * m_buttons.size() + INSET * 2);
        m_boxSize.height += maxHeight + V_GAP;
        
        float h = (float)m_buttons.size() / 2.f;
        h = (sz.width * 0.5f) - (h - .5f) * (maxWidth + H_GAP);
        int i = 0;
        for (auto it = m_buttons.begin(); it != m_buttons.end(); ++it, ++i)
        {
            (*it)->setPreferredSize(Size(maxWidth, maxHeight));
            (*it)->setPosition(Point(h + (maxWidth + H_GAP) * i, 0.5f * (sz.height - m_boxSize.height) + INSET));
        }
    }
    
    
    Scale9Sprite* bkg = (Scale9Sprite*)getChildByTag(TAG_BKG);
    CC_ASSERT(bkg);
    bkg->setPreferredSize(m_boxSize);

    if (title)
    {
        title->setPosition(Point((sz.width - m_boxSize.width) * .5f + INSET, 0.5f * (sz.height - m_boxSize.height) + m_boxSize.height - INSET));
    }
    if (msg)
    {
        msg->setPosition(Point(sz) * 0.5f);
    }
}



bool CMessageBoxLayer::onTouchBegan(Touch *touch, Event *event)
{
    return true;
}

