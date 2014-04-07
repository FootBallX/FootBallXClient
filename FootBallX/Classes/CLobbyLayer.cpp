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
//#include "CPlayerInfo.h"

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
}



CLobbyLayer::~CLobbyLayer()
{
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
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "logText", LabelTTF*, m_logText);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnSignUp", ControlButton*, m_singUpButton);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "btnCancel", ControlButton*, m_cancelButton);
    return false;
}


void CLobbyLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
{
    m_logText->setString("click \"Sign Up\"");
}




bool CLobbyLayer::init()
{
    auto ret = POMELO->addListener("onPair", [&](Node* node, void* resp) -> void
    {
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        json_t* code = json_object_get(ccpomeloresp->docs, "code");
        if (200 != json_integer_value(code))
        {
            log("onPair fail");
            return;
        }
        
        json_t* token = json_object_get(ccpomeloresp->docs, "token");
        connectMatchServer(json_integer_value(token));
    });
    CC_ASSERT(ret == 0);
    
    ret = POMELO->addListener("startMatch", [&](Node* node, void* resp) -> void
    {
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        json_t* code = json_object_get(ccpomeloresp->docs, "code");
        if (200 != json_integer_value(code))
        {
            return;
        }
        
        log("start match");
    });
    CC_ASSERT(ret == 0);
    
	return true;
}



void CLobbyLayer::connectMatchServer(json_int_t token)
{
    const char *route = "match.matchHandler.ready";
    json_t *msg = json_object();
    json_object_set(msg, "token", json_integer(token));
    POMELO->request(route, msg, [&](Node* node, void* resp){
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        json_t* code = json_object_get(ccpomeloresp->docs, "code");
        if (200 != json_integer_value(code))
        {
            return;
        }
    });
}


void CLobbyLayer::onSignUp(Ref* sender, Control::EventType event)
{
    const char *route = "league.leagueHandler.signUp";
    json_t *msg = json_object();

    POMELO->request(route, msg, [&](Node* node, void* resp){
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        json_t* code = json_object_get(ccpomeloresp->docs, "code");
        if (200 != json_integer_value(code))
        {
            return;
        }
        m_logText->setString("You are in queue, please wait for other player.");
        m_singUpButton->setVisible(false);
    });
}



void CLobbyLayer::onCancel(Ref* sender, Control::EventType event)
{
    SCENE_MANAGER->go(ST_LOGIN);
}

