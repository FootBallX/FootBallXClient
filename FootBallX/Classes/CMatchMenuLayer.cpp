//
//  CMatchMenuLayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-19.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CMatchMenuLayer.h"
#include "CMatchLayer.h"
#include "CCardLayer.h"
#include "CMenuFrameLayer.h"

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



void CMatchMenuLayer::onEnter()
{
    CBaseLayer::onEnter();
}



void CMatchMenuLayer::onExit()
{
    CBaseLayer::onExit();
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
    // AUTO_GEN_MENU_ITEM_BEGIN
    // AUTO_GEN_MENU_ITEM_END
    return nullptr;
}



Control::Handler CMatchMenuLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_CONTROL_BEGIN
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onDribble", CMatchMenuLayer::onDribble);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onPass", CMatchMenuLayer::onPass);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onShoot", CMatchMenuLayer::onShoot);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onOneTwo", CMatchMenuLayer::onOneTwo);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onClear", CMatchMenuLayer::onClear);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTackle", CMatchMenuLayer::onTackle);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onIntercept", CMatchMenuLayer::onIntercept);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBlock", CMatchMenuLayer::onBlock);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCatch", CMatchMenuLayer::onCatch);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onHit", CMatchMenuLayer::onHit);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBlockDribble", CMatchMenuLayer::onBlockDribble);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onBlockShoot", CMatchMenuLayer::onBlockShoot);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onAttack", CMatchMenuLayer::onAttack);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onWait", CMatchMenuLayer::onWait);
    // AUTO_GEN_CONTROL_END
    return nullptr;
}


bool CMatchMenuLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node * pNode)
{
    // AUTO_GEN_VAR_BEGIN
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "menuFrameLayer", CMenuFrameLayer*, this->m_menuFrameLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonDribble", ControlButton*, this->m_varButtonDribble);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonPass", ControlButton*, this->m_varButtonPass);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonShoot", ControlButton*, this->m_varButtonShoot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonOneTwo", ControlButton*, this->m_varButtonOneTwo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonClear", ControlButton*, this->m_varButtonClear);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonTackle", ControlButton*, this->m_varButtonTackle);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonIntercept", ControlButton*, this->m_varButtonIntercept);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonBlock", ControlButton*, this->m_varButtonBlock);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonCatch", ControlButton*, this->m_varButtonCatch);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonHit", ControlButton*, this->m_varButtonHit);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonBlockDribble", ControlButton*, this->m_varButtonBlockDribble);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonBlockShoot", ControlButton*, this->m_varButtonBlockShoot);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonAttack", ControlButton*, this->m_varButtonAttack);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "varButtonWait", ControlButton*, this->m_varButtonWait);
    // AUTO_GEN_VAR_END
	return false;
}



void CMatchMenuLayer::setPlayers(const vector<int>& ap, const vector<int>& dp)
{
    m_attackPlayerNumbers = ap;
    m_defendPlayerNumbers = dp;
    
    m_menuFrameLayer->setPlayers(ap, dp);
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



void CMatchMenuLayer::onClear(Ref* sender, Control::EventType event)
{

}


void CMatchMenuLayer::onCatch(Ref* sender, Control::EventType event)
{

}


void CMatchMenuLayer::onBlockDribble(Ref* sender, Control::EventType event)
{

}


void CMatchMenuLayer::onBlockShoot(Ref* sender, Control::EventType event)
{

}


void CMatchMenuLayer::onAttack(Ref* sender, Control::EventType event)
{

}


void CMatchMenuLayer::onWait(Ref* sender, Control::EventType event)
{

}


