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
        NodeLoaderLibrary::getInstance()->registerNodeLoader( "CMatchLayer", CMatchLayerLoader::loader());
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
    //    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "helloLabel", CCLabelTTF*, helloLabel);
    return false;
}


void CMatchLayer::onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader)
{
    
}