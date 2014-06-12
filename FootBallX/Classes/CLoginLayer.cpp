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


void CLoginLayer::onEnter()
{
    CBaseLayer::onEnter();
}

void CLoginLayer::onExit()
{
    CBaseLayer::onExit();
}


SEL_MenuHandler CLoginLayer::onResolveCCBCCMenuItemSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_MENU_ITEM_BEGIN
    // AUTO_GEN_MENU_ITEM_END
    return nullptr;
}

//函数定义类型为：void pressTitle(Ref*pSender);
Control::Handler CLoginLayer::onResolveCCBCCControlSelector(Ref* pTarget, const char* pSelectorName)
{
    // AUTO_GEN_CONTROL_BEGIN
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onLogin", CLoginLayer::onLogin);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onCancel", CLoginLayer::onCancel);
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE(this, "onTest", CLoginLayer::onTest);
    // AUTO_GEN_CONTROL_END
    return nullptr;
}

bool CLoginLayer::onAssignCCBMemberVariable(Ref* pTarget, const char* pMemberVariableName, Node* pNode)
{
    // AUTO_GEN_VAR_BEGIN
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "editUserNameLayer", Layer*, this->m_editUserNameLayer);
    CCB_MEMBERVARIABLEASSIGNER_GLUE_WEAK(this, "editPasswordLayer", Layer*, this->m_editPasswordLayer);
    // AUTO_GEN_VAR_END
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
#else
    m_userName->setText("test2");
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
//    std::string ip = "192.168.1.6";
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
                             CJsonT msg;
                             msg.setChild("userName", m_userName->getText());
                             msg.setChild("password", m_password->getText());

                             POMELO->request(route, msg, [&, mb](Node* node, void* resp){
                                 mb->closeModal();
                                 CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                                 
                                 CJsonT docs(ccpomeloresp->docs);
                                 
                                 CCLOG("entryCB %s", docs.dump().c_str());

                                 if (200 != docs.getInt("code"))
                                 {
                                     return;
                                 }

                                 connectToConnector(docs.getString("host"), docs.getInt("port"));
                             });
                             
                             msg.release();
                         });
}



void CLoginLayer::onCancel(Ref* sender, Control::EventType event)
{
    POMELO->stop();
    delete POMELO;
    json_memory_dump();
    
    Director::getInstance()->end();
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

                                      CJsonT msg;
                                      msg.setChild("userName", m_userName->getText());
                                      msg.setChild("password", m_password->getText());
                                      POMELO->request(route, msg, [&, mb](Node* node, void* resp){
                                          mb->closeModal();
                                          CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                                          CJsonT docs(ccpomeloresp->docs);

                                          CCLOG("entryCB %s", docs.dump().c_str());

                                          if (docs.getInt("code") != 200)
                                          {
                                              CCLOG("connect to connector failed");
                                          }
                                          else
                                          {
                                              CCLOG("connect to connector ok");
                                              getPlayerInfo();
                                          }
                                      });
                                      msg.release();
                                  });
}


void CLoginLayer::getPlayerInfo(void)
{
    CMessageBoxLayer* mb = CMessageBoxLayer::create();
    mb->setMsg("Getting player info ...");
    mb->doModal();
    const char *route = "connector.entryHandler.getPlayerInfo";;
    CJsonT msg;
    POMELO->request(route, msg, [&, mb](Node* node, void* resp)
    {
        mb->closeModal();
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        CJsonT docs(ccpomeloresp->docs);

        if (docs.getInt("code") != 200)
        {
            CCLOG("get player info failed");
        }
        else
        {
            CCLOG("get player info done!");
            CCLOG("playerInfo: %s", docs.dump().c_str());

            CJsonT player(docs.getChild("player"));
            
            PLAYER_INFO->setUID(player.getUInt("uid"));
            PLAYER_INFO->setPID(player.getUInt("pid"));
            const char* nickname = player.getString("nickname");
            PLAYER_INFO->setNickName(nickname ? nickname : "");
            PLAYER_INFO->setLevel(player.getInt("level"));
            PLAYER_INFO->setMoney(player.getInt("money"));

            SCENE_MANAGER->go(ST_LOBBY);
        }
    });
    msg.release();
}


// Codes below are for testing.

#include "CMatchMenuLayer.h"
#include "CCBReadHelper.h"

void CLoginLayer::onTest(Ref* sender, Control::EventType event)
{
//    SCENE_MANAGER->go(ST_MATCH);

        auto m = CCBReadHelper::read("fb_menu_1.ccbi");
        addChild(m);
        
        CMatchMenuLayer* p = dynamic_cast<CMatchMenuLayer*>(m);
        p->setPlayers({10}, {1,2,3,4}, 0);
    
    
    auto node = CCBReadHelper::read("air_chuanqiu.ccbi");
    this->addChild(node);
}





