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
#include "CPlayerInfo.h"

static class CLoginLayerRegister
{
public:
    CLoginLayerRegister()
    {
        cocosbuilder::NodeLoaderLibrary::getInstance()->registerNodeLoader( "CLoginLayer", CLoginLayerLoader::loader());
    }
} __reg;



CLoginLayer::CLoginLayer()
{
}



CLoginLayer::~CLoginLayer()
{
}



SEL_MenuHandler CLoginLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onLogin", CLoginLayer::onLogin);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onCancel", CLoginLayer::onCancel);
//    CCB_SELECTORRESOLVER_CCMENUITEM_GLUE(this, "onTestSocket", CLobbyLayer::onTestSocket);
    return nullptr;
}

//函数定义类型为：void pressTitle(Ref*pSender);
Control::Handler CLoginLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLogin", CLoginLayer::onLogin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancel", CLoginLayer::onCancel);
//    CCB_SELECTORRESOLVER_CCCONTROL_GLUE( this, "pressTitle", CLobbyLayer::controlButtonTest);
    return nullptr;
}

bool CLoginLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_edtUserNameLayer", Layer*, m_editUserNameLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_edtPasswordLayer", Layer*, m_editPasswordLayer);
    return false;
}


void CLoginLayer::onNodeLoaded(Node * pNode, cocosbuilder::NodeLoader * pNodeLoader)
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
    POMELO->stop();
    delete POMELO;
    
    SpriteFrameCache* sfc = SpriteFrameCache::getInstance();
    sfc->addSpriteFramesWithFile("huds.plist");

	return true;
}



void CLoginLayer::onLogin(Ref* sender, Control::EventType event)
{
    std::string ip = "127.0.0.1";
    int port = 3017;

    CMessageBoxLayer* mb = CMessageBoxLayer::create();
    mb->setMsg("Connecting...");
    mb->doModal();

    POMELO->asyncConnect(ip.c_str(), port, [&, mb](Node* node, void* resp)
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
                             auto jo = json_string(m_userName->getText());
                             json_object_set(msg, "userName", jo);
                             json_decref(jo);
                             jo = json_string(m_password->getText());
                             json_object_set(msg, "password", jo);
                             json_decref(jo);
                             POMELO->request(route, msg, [&, mb](Node* node, void* resp){
                                 mb->closeModal();
                                 CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
#if COCOS2D_DEBUG == 1
                                 auto jds = json_dumps(ccpomeloresp->docs, JSON_COMPACT);
                                 CCLOG("entryCB %s", jds);
                                 free(jds);
#endif
                                 json_t* code = json_object_get(ccpomeloresp->docs, "code");
                                 if (200 != json_integer_value(code))
                                 {
                                     return;
                                 }
                                 json_t* host = json_object_get(ccpomeloresp->docs, "host");
                                 json_t* ip = json_object_get(ccpomeloresp->docs, "port");
                                 connectToConnector(json_string_value(host), (int)json_integer_value(ip));
                             });
                         });
}



void CLoginLayer::onCancel(Ref* sender, Control::EventType event)
{
    SCENE_MANAGER->go(ST_MATCH);
}



void CLoginLayer::connectToConnector(const char* ip, int port)
{
    CMessageBoxLayer* mb = CMessageBoxLayer::create();
    mb->setMsg("Entering game ...");
    mb->doModal();
    POMELO->stop();
    POMELO->asyncConnect(ip, port, [&, mb](Node* node, void* resp)
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
                                      auto jo = json_string(m_userName->getText());
                                      json_object_set(msg, "userName", jo);
                                      json_decref(jo);
                                      jo = json_string(m_password->getText());
                                      json_object_set(msg, "password", jo);
                                      json_decref(jo);
                                      POMELO->request(route, msg, [&, mb](Node* node, void* resp){
                                          mb->closeModal();
                                          CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
#if COCOS2D_DEBUG == 1
                                          auto jds = json_dumps(ccpomeloresp->docs, JSON_COMPACT);
                                          CCLOG("entryCB %s", jds);
                                          free(jds);
#endif
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
                                  });
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
#if COCOS2D_DEBUG == 1
            auto jds = json_dumps(ccpomeloresp->docs, JSON_COMPACT);
            CCLOG("playerInfo: %s", jds);
            free(jds);
#endif
            json_t* player = json_object_get(ccpomeloresp->docs, "player");
            CC_ASSERT(player);
            PLAYER_INFO->setUID((int)json_integer_value(json_object_get(player, "uid")));
            PLAYER_INFO->setPID((int)json_integer_value(json_object_get(player, "pid")));
            const char* nickname = json_string_value(json_object_get(player, "nickname"));
            PLAYER_INFO->setNickName(nickname ? nickname : "");
            PLAYER_INFO->setLevel((int)json_integer_value(json_object_get(player, "level")));
            PLAYER_INFO->setMoney((int)json_integer_value(json_object_get(player, "money")));

            SCENE_MANAGER->go(ST_LOBBY);
        }
    });
}
