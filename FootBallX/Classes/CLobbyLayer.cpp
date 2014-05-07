//
//  CLobbyLayer.cpp
//  GameX
//
//  Created by 马 俊 on 13-11-20.
//
//

#include "CLobbyLayer.h"
#include "CCPomelo.h"
#include "CGameSceneManager.h"
#include "CMessageBoxLayer.h"
#include "CPlayerInfo.h"

static class CLobbyLayerRegister
{
public:
    CLobbyLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CLobbyLayer", CLobbyLayerLoader::loader());
    }
} __reg;



CLobbyLayer::CLobbyLayer()
{
    POMELO->addListener("onPair", bind(&CLobbyLayer::onPair, this, std::placeholders::_1, std::placeholders::_2));
}



CLobbyLayer::~CLobbyLayer()
{
    POMELO->removeListener("onPair");
}



SEL_MenuHandler CLobbyLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLogin", CLobbyLayer::onLogin);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCancel", CLobbyLayer::onCancel);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTestSocket", CLobbyLayer::onTestSocket);
    return nullptr;
}

//函数定义类型为：void pressTitle(Ref*pSender);
Control::Handler CLobbyLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onSignUp", CLobbyLayer::onSignUp);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancel", CLobbyLayer::onCancel);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE( this, "pressTitle", CLobbyLayer::controlButtonTest);
    return nullptr;
}

bool CLobbyLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
    
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "logText", decltype(m_logText), m_logText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "btnSignUp", decltype(m_signUpButton), m_signUpButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "btnCancel", decltype(m_cancelButton), m_cancelButton);
    return false;
}


void CLobbyLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
    m_logText->setString("click \"Sign Up\"");
}




bool CLobbyLayer::init()
{
    do
    {
        BREAK_IF_FAILED(CBaseLayer::init());
    } while(false);
    
	return true;
}


#pragma mark - net


void CLobbyLayer::onPair(Node* node, void* resp)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
    CJsonT docs(ccpomeloresp->docs);
    if (200 != docs.getInt("code"))
    {
        return;
    }
    
    SCENE_MANAGER->go(ST_MATCH);
}


#pragma mark - ui handler

void CLobbyLayer::onSignUp(Ref* sender, Control::EventType event)
{
    const char *route = "league.leagueHandler.signUp";

    CJsonT msg;

    POMELO->request(route, msg, [&](Node* node, void* resp){
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        CJsonT docs(ccpomeloresp->docs);
        if (200 != docs.getInt("code"))
        {
            return;
        }
        m_logText->setString("You are in queue, please wait for other player.");
        m_signUpButton->setVisible(false);
    });
    
    msg.release();
}



void CLobbyLayer::onCancel(Ref* sender, Control::EventType event)
{
    SCENE_MANAGER->go(ST_LOGIN);
}

