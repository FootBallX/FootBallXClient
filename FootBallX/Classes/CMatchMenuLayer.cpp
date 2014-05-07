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
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "VarButtonDribble", ControlButton*, m_btnDribble);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "VarButtonPass", ControlButton*, m_btnPass);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "VarButtonOneTwo", ControlButton*, m_btnOneTwo);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "VarButtonShoot", ControlButton*, m_btnShoot);
    
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerImg3", Sprite*, this->m_PlayerImg3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerImg1", Sprite*, this->m_PlayerImg1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerImg0", Sprite*, this->m_PlayerImg0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerImg2", Sprite*, this->m_PlayerImg2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerImg4", Sprite*, this->m_PlayerImg4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "DefCard4", CCardLayer*, this->m_DefCard4);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "DefCard3", CCardLayer*, this->m_DefCard3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "DefCard2", CCardLayer*, this->m_DefCard2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "DefCard1", CCardLayer*, this->m_DefCard1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "DefCard0", CCardLayer*, this->m_DefCard0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "AtkCard", CCardLayer*, this->m_AtkCard);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerNick0", Label*, this->m_PlayerNick0);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerNick1", Label*, this->m_PlayerNick1);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerNick2", Label*, this->m_PlayerNick2);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerNick3", Label*, this->m_PlayerNick3);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "PlayerNick4", Label*, this->m_PlayerNick4);
	return false;
}



void CMatchMenuLayer::setPlayers(const vector<int>& ap, const vector<int>& dp)
{
    m_attackPlayerNumbers = ap;
    m_AtkCard->setCardName(m_attackPlayerNumbers[0]);
    
    m_defendPlayerNumbers = dp;
    
    auto size = dp.size();
    
    m_DefCard0->setCardName(dp[0]);
    if (size > 1)
    {
        m_DefCard1->setCardName(dp[1]);
    }
    if (size > 2)
    {
        m_DefCard2->setCardName(dp[2]);
    }
    if (size > 3)
    {
        m_DefCard3->setCardName(dp[3]);
    }
    if (size > 4)
    {
        m_DefCard4->setCardName(dp[4]);
    }
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






