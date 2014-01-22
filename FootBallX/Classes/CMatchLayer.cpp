//
//  CMatchLayer.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-1-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CMatchLayer.h"
#include "CGameSceneManager.h"

#include "CCPomelo.h"

static class CMatchLayerRegister
{
public:
    CMatchLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMatchLayer", CMatchLayerLoader::loader());
    }
} __reg;




CMatchLayer::CMatchLayer()
{
    
}



CMatchLayer::~CMatchLayer()
{
    
}



bool CMatchLayer::init()
{
    do
    {
        return true;
    } while (false);
    
    return false;
}


SEL_MenuHandler CMatchLayer::onResolveCCBCCMenuItemSelector(Object * pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onFormation", CMatchLayer::onFormation);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onBattle", CMatchLayer::onBattle);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onHome", CMatchLayer::onHome);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTestSocket", CMatchLayer::onTestSocket);
    return nullptr;
}

//函数定义类型为：void pressTitle(Object *pSender);
Control::Handler CMatchLayer::onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName)
{
    //    CCB_SELECTORRESOLVER_CCCONTROL_GLUE( this, "pressTitle", CMatchLayer::controlButtonTest);
    return nullptr;
}

bool CMatchLayer::onAssignCCBMemberVariable(Object* pTarget, const char* pMemberVariableName, Node* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b0", Sprite*, m_blackPlayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b1", Sprite*, m_blackPlayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b2", Sprite*, m_blackPlayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b3", Sprite*, m_blackPlayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b4", Sprite*, m_blackPlayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b5", Sprite*, m_blackPlayers[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b6", Sprite*, m_blackPlayers[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b7", Sprite*, m_blackPlayers[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b8", Sprite*, m_blackPlayers[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b9", Sprite*, m_blackPlayers[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "b10", Sprite*, m_blackPlayers[10]);
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r0", Sprite*, m_redPlayers[0]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r1", Sprite*, m_redPlayers[1]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r2", Sprite*, m_redPlayers[2]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r3", Sprite*, m_redPlayers[3]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r4", Sprite*, m_redPlayers[4]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r5", Sprite*, m_redPlayers[5]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r6", Sprite*, m_redPlayers[6]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r7", Sprite*, m_redPlayers[7]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r8", Sprite*, m_redPlayers[8]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r9", Sprite*, m_redPlayers[9]);
    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "r10", Sprite*, m_redPlayers[10]);
    
    return false;
}


void CMatchLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader* pNodeLoader)
{
    for (int i = 0; i < 11; ++i)
    {
        m_blackPlayers[i]->setVisible(false);
        m_redPlayers[i]->setVisible(false);
    }
}