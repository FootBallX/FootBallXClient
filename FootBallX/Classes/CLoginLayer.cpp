//
//  CLoginLayer.cpp
//  GameX
//
//  Created by 马 俊 on 13-11-20.
//
//

#include "CLoginLayer.h"
#include "CCPomelo.h"
#include "CGameSceneManager.h"
#include "CMessageBoxLayer.h"
//#include "CPlayerInfo.h"

static class CLoginLayerRegister
{
public:
    CLoginLayerRegister()
    {
        NodeLoaderLibrary::getInstance()->registerNodeLoader( "CLoginLayer", CLoginLayerLoader::loader());
    }
} __reg;



CLoginLayer::CLoginLayer()
{
}



CLoginLayer::~CLoginLayer()
{
}



SEL_MenuHandler CLoginLayer::onResolveCCBCCMenuItemSelector(Object * pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLogin", CLoginLayer::onLogin);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCancel", CLoginLayer::onCancel);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTestSocket", CLobbyLayer::onTestSocket);
    return nullptr;
}

//函数定义类型为：void pressTitle(Object *pSender);
Control::Handler CLoginLayer::onResolveCCBCCControlSelector(Object * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLogin", CLoginLayer::onLogin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancel", CLoginLayer::onCancel);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE( this, "pressTitle", CLobbyLayer::controlButtonTest);
    return nullptr;
}

bool CLoginLayer::onAssignCCBMemberVariable(Object* pTarget, const char* pMemberVariableName, Node* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_edtUserNameLayer", Layer*, m_editUserNameLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_edtPasswordLayer", Layer*, m_editPasswordLayer);
    return false;
}


void CLoginLayer::onNodeLoaded(Node * pNode, NodeLoader * pNodeLoader)
{
    m_userName = createEdit("huds/hp_bar_bg0.png", m_editUserNameLayer->getContentSize());
    CC_ASSERT(m_userName);
    m_userName->setAnchorPoint(Point(0.f, 0.f));
    m_editUserNameLayer->addChild(m_userName);
    m_userName->setInputMode(EditBox::InputMode::SINGLE_LINE);

    m_password = createEdit("huds/hp_bar_bg0.png", m_editPasswordLayer->getContentSize());
    CC_ASSERT(m_password);
    m_password->setAnchorPoint(Point(0.f, 0.f));
    m_editPasswordLayer->addChild(m_password);
    m_password->setInputMode(EditBox::InputMode::SINGLE_LINE);
    m_password->setInputFlag(EditBox::InputFlag::PASSWORD);
    
#if DEBUG
    m_userName->setText("test1");
    m_password->setText("123");
#endif

}




bool CLoginLayer::init()
{
    SpriteFrameCache* sfc = SpriteFrameCache::getInstance();
    sfc->addSpriteFramesWithFile("huds.plist");

	return true;
}



void CLoginLayer::onLogin(Object* sender, Control::EventType event)
{
    std::string ip = "127.0.0.1";
    int port = 3017;

    CMessageBoxLayer* mb = CMessageBoxLayer::create();
    mb->setMsg("Connecting...");
    mb->doModal();

    if (0 != POMELO->asyncConnect(ip.c_str(), port, [&, mb](Node* node, void* resp)
                         {
                             CCPomeloReponse* pr = (CCPomeloReponse*)resp;
                             if (pr->status != 0)
                             {
                                 CCLOG("not connected!");
                                 mb->closeModal();
                                 return;
                             }
                             CCLOG("connect ok");
                             const char *route = "gate.gateHandler.queryConnectorEntry";
                             json_t *msg = json_object();
                             json_object_set(msg, "userName", json_string(m_userName->getText()));
                             json_object_set(msg, "password", json_string(m_password->getText()));
                             POMELO->request(route, msg, [&, mb](Node* node, void* resp){
                                 mb->closeModal();
                                 CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                                 CCLOG("entryCB %s",json_dumps(ccpomeloresp->docs, JSON_COMPACT));
                                 
                                 json_t* code = json_object_get(ccpomeloresp->docs, "code");
                                 if (200 != json_integer_value(code))
                                 {
                                     return;
                                 }
                                 json_t* host = json_object_get(ccpomeloresp->docs, "host");
                                 json_t* ip = json_object_get(ccpomeloresp->docs, "port");
                                 connectToConnector(json_string_value(host), json_integer_value(ip));
                             });
                         }))
    {
        mb->closeModal();
    }
}



void CLoginLayer::onCancel(Object* sender, Control::EventType event)
{
    SCENE_MANAGER->go(ST_MATCH);
}



void CLoginLayer::connectToConnector(const char* ip, int port)
{
    CMessageBoxLayer* mb = CMessageBoxLayer::create();
    mb->setMsg("Entering game ...");
    mb->doModal();
    
    if (0 != POMELO->asyncConnect(ip, port, [&, mb](Node* node, void* resp)
                                  {
                                      CCPomeloReponse* pr = (CCPomeloReponse*)resp;
                                      if (pr->status != 0)
                                      {
                                          CCLOG("gate not connected!");
                                          mb->closeModal();
                                          return;
                                      }
                                      CCLOG("gate connect ok");
                                      const char *route = "connector.entryHandler.login";;
                                      json_t *msg = json_object();
                                      json_object_set(msg, "userName", json_string(m_userName->getText()));
                                      json_object_set(msg, "password", json_string(m_password->getText()));
                                      POMELO->request(route, msg, [&, mb](Node* node, void* resp){
                                          mb->closeModal();
                                          CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                                          CCLOG("entryCB %s",json_dumps(ccpomeloresp->docs, JSON_COMPACT));
                                          
                                          json_t* code = json_object_get(ccpomeloresp->docs, "code");
                                          if (json_integer_value(code) != 200)
                                          {
                                              CCLOG("connect to connector failed");
                                          }
                                          else
                                          {
                                              CCLOG("connect to connector ok");
                                              getPlayerInfo();
                                          }
                                      });
                                  }))
    {
        mb->closeModal();
    }
}


void CLoginLayer::getPlayerInfo(void)
{
    CMessageBoxLayer* mb = CMessageBoxLayer::create();
    mb->setMsg("Getting player info ...");
    mb->doModal();
    const char *route = "connector.entryHandler.getPlayerInfo";;
    json_t *msg = json_object();
    POMELO->request(route, msg, [&, mb](Node* node, void* resp)
    {
        mb->closeModal();
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        
        json_t* code = json_object_get(ccpomeloresp->docs, "code");
        if (json_integer_value(code) != 200)
        {
            CCLOG("get player info failed");
        }
        else
        {
            CCLOG("get player info done!");
            CCLOG("playerInfo: %s",json_dumps(ccpomeloresp->docs, JSON_COMPACT));
            json_t* player = json_object_get(ccpomeloresp->docs, "player");
            CC_ASSERT(player);
//            PLAYER_INFO->setUID(json_integer_value(json_object_get(player, "uid")));
//            PLAYER_INFO->setPID(json_integer_value(json_object_get(player, "pid")));
            const char* nickname = json_string_value(json_object_get(player, "nickname"));
//            PLAYER_INFO->setNickName(nickname ? nickname : "");
//            PLAYER_INFO->setLevel(json_integer_value(json_object_get(player, "level")));
//            PLAYER_INFO->setMoney(json_integer_value(json_object_get(player, "money")));
//            PLAYER_INFO->setMineral(json_integer_value(json_object_get(player, "mineral")));
//            PLAYER_INFO->setGas(json_integer_value(json_object_get(player, "gas")));
            
            SCENE_MANAGER->go(ST_MATCH);
        }
    });
}
