//
//  CMatchMenuLayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-19.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CMatchMenuLayer.h"
#include "CMatchLayer.h"

static class CMatchMenuLayerRegister
{
public:
    CMatchMenuLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMatchMenuLayer", CMatchMenuLayerLoader::loader());
    }
} __reg;



CMatchMenuLayer::CMatchMenuLayer() : m_matchLayer(nullptr)
{
}



CMatchMenuLayer::~CMatchMenuLayer()
{
}



bool CMatchMenuLayer::init()
{
    do
    {
        BREAK_IF_FAILED(CBaseLayer::init());
        return true;
    } while (false);
    
	return false;
}



void CMatchMenuLayer::update(float dt)
{
}



SEL_MenuHandler CMatchMenuLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{

    return nullptr;
}



Control::Handler CMatchMenuLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonPass", CMatchMenuLayer::onPass);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonDribble", CMatchMenuLayer::onDribble);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonOneTwo", CMatchMenuLayer::onOneTwo);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonShoot", CMatchMenuLayer::onShoot);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonTackle", CMatchMenuLayer::onTackle);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonIntercept", CMatchMenuLayer::onIntercept);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonBlock", CMatchMenuLayer::onBlock);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "MenuButtonHit", CMatchMenuLayer::onHit);
    return nullptr;
}



bool CMatchMenuLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "VarButtonDribble", ControlButton*, m_btnDribble);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "VarButtonPass", ControlButton*, m_btnPass);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "VarButtonOneTwo", ControlButton*, m_btnOneTwo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "VarButtonShoot", ControlButton*, m_btnShoot);
	return false;
}



void CMatchMenuLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
}



void CMatchMenuLayer::onPass(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onPass(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onDribble(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onDribble(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onShoot(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onShoot(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onOneTwo(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onOneTwo(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onTackle(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onTackle(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onBlock(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onBlock(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onIntercept(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onIntercept(sender);
            break;
        default:
            break;
    }
}



void CMatchMenuLayer::onHit(Ref* sender, Control::EventType event)
{
    switch (event)
    {
        case Control::EventType::TOUCH_DOWN:
            break;
        case Control::EventType::TOUCH_UP_INSIDE:
            m_matchLayer->onHit(sender);
            break;
        default:
            break;
    }
}






