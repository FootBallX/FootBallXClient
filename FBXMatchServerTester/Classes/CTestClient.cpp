//
//  CTestClient.cpp
//  FBXMatchServerTester
//
//  Created by 马 俊 on 14-5-30.
//
//

#include "CTestClient.h"
#include "CFBPlayerInitInfo.h"
#include "CTestCaseManager.h"
#include "CMatchTester.h"

CTestClient::~CTestClient()
{

}


void CTestClient::addAllListener()
{
    m_pomelo->addListener("onPair", bind(&CTestClient::onPair, this, std::placeholders::_1, std::placeholders::_2));
    m_pomelo->addListener("startMatch", bind(&CTestClient::onStartMatch, this, std::placeholders::_1, std::placeholders::_2));
    m_pomelo->addListener("triggerMenu", std::bind(&CTestClient::onTriggerMenu, this, std::placeholders::_1, std::placeholders::_2));
    m_pomelo->addListener("instructions", std::bind(&CTestClient::onInstructionResult, this, std::placeholders::_1, std::placeholders::_2));
    m_pomelo->addListener("resumeMatch", std::bind(&CTestClient::onResumeMatch, this, std::placeholders::_1, std::placeholders::_2));

}



void CTestClient::removeAllListener()
{
    if (m_pomelo)
    {
        m_pomelo->removeListener("onPair");
        m_pomelo->removeListener("startMatch");
    }
}



bool CTestClient::init(const char* username, const char* password)
{
    do
    {
        BREAK_IF(nullptr == username);
        BREAK_IF(nullptr == password);
        m_username = username;
        m_password = password;
        
        int index = POMELO_NEW;
        m_pomelo = POMELO(index);
        BREAK_IF(nullptr == m_pomelo);
        
        std::string ip = "127.0.0.1";
        int port = 3017;
        
        m_pomelo->asyncConnect(ip.c_str(), port, [&](Node* node, void* resp)
                               {
                                   CCPomeloReponse* pr = (CCPomeloReponse*)resp;
                                   if (pr->status != 0)
                                   {
                                       MT->print("not connected!");
                                       return;
                                   }
                                   CCLOG("connect ok");
                                   const char *route = "gate.gateHandler.queryConnectorEntry";
                                   CJsonT msg;
                                   msg.setChild("userName", m_username.c_str());
                                   msg.setChild("password", m_password.c_str());
                                   
                                   m_pomelo->request(route, msg, [&](Node* node, void* resp){
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
        
        return true;
    } while (false);
    
    return false;
}


void CTestClient::connectToConnector(const char* ip, int port)
{
    m_pomelo->stop();
    m_pomelo->asyncConnect(ip, port, [&](Node* node, void* resp)
                           {
                               CCPomeloReponse* pr = (CCPomeloReponse*)resp;
                               if (pr->status != 0)
                               {
                                   MT->print("gate not connected!");
                                   return;
                               }
                               CCLOG("gate connect ok");
                               const char *route = "connector.entryHandler.login";;
                               
                               CJsonT msg;
                               msg.setChild("userName", m_username.c_str());
                               msg.setChild("password", m_password.c_str());
                               m_pomelo->request(route, msg, [&](Node* node, void* resp){
                                   CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                                   CJsonT docs(ccpomeloresp->docs);
                                   
                                   CCLOG("entryCB %s", docs.dump().c_str());
                                   
                                   if (docs.getInt("code") != 200)
                                   {
                                       MT->print("connect to connector failed");
                                   }
                                   else
                                   {
                                       CCLOG("connect to connector ok");
                                       
                                       addAllListener();
                                       getPlayerInfo();
                                   }
                               });
                               msg.release();
                           });
}



void CTestClient::getPlayerInfo(void)
{
    const char *route = "connector.entryHandler.getPlayerInfo";;
    CJsonT msg;
    m_pomelo->request(route, msg, [&](Node* node, void* resp)
                      {
                          CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                          CJsonT docs(ccpomeloresp->docs);
                          CCLOG("playerInfo: %s", docs.dump().c_str());
                          
                          if (docs.getInt("code") != 200)
                          {
                              MT->print("get player info failed");
                          }
                          else
                          {
                              CCLOG("get player info done!");
                              CCLOG("playerInfo: %s", docs.dump().c_str());
                              
                              CJsonT player(docs.getChild("player"));
                              
                              m_playerInfo.setUID(player.getUInt("uid"));
                              const char* nickname = player.getString("nickname");
                              m_playerInfo.setNickName(nickname ? nickname : "");
                              m_playerInfo.setLevel(player.getInt("level"));
                              m_playerInfo.setMoney(player.getInt("money"));
                              
                              m_state = STATE::LoginReady;
                          }
                      });
    msg.release();
}



void CTestClient::onPair(Node* node, void* resp)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
    CJsonT docs(ccpomeloresp->docs);
    if (200 != docs.getInt("code"))
    {
        return;
    }
    
    m_state = STATE::Paired;
}



void CTestClient::onTriggerMenu(Node* node, void* resp)
{
    MT->print("case encounter");
    
    m_caseState = CASE_STATE::Encounter;
    
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
    CJsonT docs(ccpomeloresp->docs);
    
    int type = docs.getInt("menuType");
    if (type == (int)FBDefs::MENU_TYPE::NONE)
    {
        return;
    }
    
    CC_ASSERT(type >= 0 && type < (unsigned int)FBDefs::MENU_TYPE::NONE);
    
    
    json_t* players = nullptr;

    decltype(m_testCase->m_atk)* tpInfo = nullptr;
    if (isAttacking())
    {
        players = docs.getChild("attackPlayers");
        tpInfo = &(m_testCase->m_atk);
        CCLOG("attack");
    }
    else
    {
        players = docs.getChild("defendplayers");
        tpInfo = &(m_testCase->m_def);
        CCLOG("def");
    }
    CJsonTArray ja(players);
    
    for (int i = 0; i < ja.size(); ++i)
    {
        auto playerId = ja.get(i).toInt();
        MT->print("playerId: %d", playerId);
        for (auto info : *tpInfo)
        {
            if (info->playerNumber == playerId)
            {
                // 发送命令
                CJsonT msg;
                if (isAttacking())
                {
                    MT->printA("%s", CTestPlayerInfo::ins_str[(int)info->instruction].c_str());
                }
                log("ins: %d", (unsigned int)info->instruction);
                msg.setChild("cmd", (unsigned int)info->instruction);
                if (isAttacking() && (*tpInfo).size() > 1)
                {
                    msg.setChild("targetPlayerId", (unsigned int)(*tpInfo)[1]->playerNumber);
                }
                
                m_pomelo->request("match.matchHandler.menuCmd", msg, [this](Node* node, void* resp){
                    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                    CJsonT docs(ccpomeloresp->docs);
                });
                msg.release();
                break;
            }
        }
    }
}



void CTestClient::onStartMatch(Node* node, void* resp)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
    CJsonT docs(ccpomeloresp->docs);

    MT->print("start match!");
    m_state = STATE::Running;
    m_syncTime = 0.f;
}



void CTestClient::onInstructionResult(Node* node, void* resp)
{
    if (isAttacking())
    {
        MT->printA("movie end");
    }
    else
    {
        MT->printD("movie end");
    }
    CJsonT msg;
    m_pomelo->notify("match.matchHandler.instructionMovieEnd", msg, [](Node* node, void* resp){});
    msg.release();
}



void CTestClient::onResumeMatch(Node* node, void* resp)
{
    if (isAttacking())
    {
        MT->printA("resume match");
    }
    else
    {
        MT->printD("resume match");
    }
    log("case done");
    m_caseState = CASE_STATE::None;
}


void CTestClient::signUp()
{
    const char *route = "league.leagueHandler.signUp";
    
    CJsonT msg;
    
    m_pomelo->request(route, msg, [&](Node* node, void* resp){
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        CJsonT docs(ccpomeloresp->docs);
        if (200 != docs.getInt("code"))
        {
            return;
        }
    });
    
    msg.release();
    
    m_state = STATE::SignedUp;
}



void CTestClient::getMatchInfo()
{
    m_state = STATE::PrepareMatch;
    const char *route = "match.matchHandler.getMatchInfo";
    CJsonT msg;
    m_pomelo->request(route, msg, std::bind(&CTestClient::onGetMatchInfo, this, std::placeholders::_1, std::placeholders::_2));
    msg.release();
}



void CTestClient::onGetMatchInfo(Node*, void* r)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)r;
    CJsonT docs(ccpomeloresp->docs);
    
    CJsonTArray left(docs.getChild("left"));
    CJsonTArray right(docs.getChild("right"));
    
    unsigned int u1 = docs.getUInt("leftUid");
    unsigned int u2 = docs.getUInt("rightUid");
    
    FBDefs::SIDE kickOffSide = FBDefs::SIDE::LEFT;
    
    if (u1 == m_playerInfo.getUID())
    {
        m_side = FBDefs::SIDE::LEFT;
    }
    else if (u2 == m_playerInfo.getUID())
    {
        m_side = FBDefs::SIDE::RIGHT;
    }
    else
    {
        CC_ASSERT(false);
    }
    
    if ( 1 == docs.getInt("kickOffSide"))
    {
        kickOffSide = FBDefs::SIDE::RIGHT;
    }
    
    if (kickOffSide == m_side)
    {
        m_ballPos = docs.getInt("kickOffPlayer");
    }
    
    int size = (int)left.size();
    CC_ASSERT(size == right.size());
    
    CFBPlayerInitInfo info;
    for (int i = 0; i < size; ++i)
    {
        {
            CJsonT player(left.get(i));
            auto& card = info.card;
            card.m_cardID = player.getUInt("pcId");
            card.m_speed = player.getFloat("speed");
            strncpy(card.m_icon, player.getString("icon"), FBDefs::MAX_CARD_ICON_LEN - 1);
            card.m_strength = player.getFloat("strength");
            card.m_dribbleSkill = player.getFloat("dribbleSkill");
            card.m_passSkill = player.getFloat("passSkill");
            card.m_shootSkill = player.getFloat("shootSkill");
            card.m_defenceSkill = player.getFloat("defenceSkill");
            card.m_attackSkill = player.getFloat("attackSkill");
            card.m_groundSkill = player.getFloat("groundSkill");
            card.m_airSkill = player.getFloat("airSkill");
            CJsonT position(player.getChild("position"));
            info.position.x = position.getFloat("x");
            info.position.y = position.getFloat("y");
            CJsonT homePosition(player.getChild("homePosition"));
            info.homePosition.x = homePosition.getFloat("x");
            info.homePosition.y = homePosition.getFloat("y");
            info.aiClass = player.getInt("aiClass");
        }
        
        {
            CJsonT player(right.get(i));
            auto& card = info.card;
            card.m_cardID = player.getUInt("pcId");
            card.m_speed = player.getFloat("speed");
            strncpy(card.m_icon, player.getString("icon"), FBDefs::MAX_CARD_ICON_LEN - 1);
            card.m_strength = player.getFloat("strength");
            card.m_dribbleSkill = player.getFloat("dribbleSkill");
            card.m_passSkill = player.getFloat("passSkill");
            card.m_shootSkill = player.getFloat("shootSkill");
            card.m_defenceSkill = player.getFloat("defenceSkill");
            card.m_attackSkill = player.getFloat("attackSkill");
            card.m_groundSkill = player.getFloat("groundSkill");
            card.m_airSkill = player.getFloat("airSkill");
            CJsonT position(player.getChild("position"));
            info.position.x = position.getFloat("x");
            info.position.y = position.getFloat("y");
            CJsonT homePosition(player.getChild("homePosition"));
            info.homePosition.x = homePosition.getFloat("x");
            info.homePosition.y = homePosition.getFloat("y");
            info.aiClass = player.getInt("aiClass");
            
        }
        
    }
    
    MT->print("uid: %ud is ready", m_playerInfo.getUID());
    const char *route = "match.matchHandler.ready";
    CJsonT msg;
    m_pomelo->notify(route, msg, [](Node* node, void* resp){});
    msg.release();
}



void CTestClient::run(float dt)
{
    if (m_state == STATE::Running)
    {
        switch (m_caseState)
        {
            case CASE_STATE::Start:
                syncCase();
                if (isAttacking() && m_testCase->active)
                {
                    CJsonT msg;
                    msg.setChild("cmd", (unsigned int)m_testCase->m_atk[0]->instruction);
                    if (isAttacking() && m_testCase->m_atk.size() > 1)
                    {
                        msg.setChild("targetPlayerId", (unsigned int)m_testCase->m_atk[1]->playerNumber);
                    }
                    
                    m_pomelo->request("match.matchHandler.menuCmd", msg, [this](Node* node, void* resp){
                        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
                        CJsonT docs(ccpomeloresp->docs);
                    });
                    msg.release();
                }
                
                m_caseState = CASE_STATE::WaitEncounter;
                break;
            case CASE_STATE::WaitEncounter:
                break;
            case CASE_STATE::Encounter:
                break;
            case CASE_STATE::None:
                m_syncTime -= dt;
                if (m_syncTime <= 0)
                {
                    syncNormal();
                    m_syncTime = 1.f;
                }
                break;
        }
    }
}



void CTestClient::close()
{
    m_state = STATE::None;
    m_pomelo->stop();
}



void CTestClient::syncNormal()
{
    CJsonT msg;
    
    CJsonTArray ja;
    for (int i = 0; i < 11; ++i)
    {
        switch (m_side)
        {
            case FBDefs::SIDE::LEFT:
                ja.append(CJsonT((float)0));
                ja.append(CJsonT((float)0));
                ja.append(CJsonT((float)0));
                ja.append(CJsonT((float)0));
                break;
            case FBDefs::SIDE::RIGHT:
                ja.append(CJsonT((float)FBDefs::PITCH_WIDTH-1));
                ja.append(CJsonT((float)FBDefs::PITCH_WIDTH-1));
                ja.append(CJsonT((float)0));
                ja.append(CJsonT((float)0));
                break;
            default:
                break;
        }
        
    }
    
    msg.setChild("teamPos", ja);
    msg.setChild("side", (int)m_side);
    msg.setChild("ballPosPlayerId", m_ballPos);
    msg.setChild("timeStamp", 0);
    m_pomelo->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    
    msg.release();
}



void CTestClient::startCase(CTestCase* tc)
{
    CCLOG("start case");
    m_testCase = tc;
    
    if (isAttacking())
    {
        CJsonT msg;
        msg.setChild("side", (int)m_side);
        msg.setChild("playerNumber", tc->ballPos);
        m_pomelo->notify("match.matchHandler.setBall", msg, [&](Node* node, void* resp){
            m_caseState = CASE_STATE::Start;
        });

        msg.release();
    }
    else
    {
        m_caseState = CASE_STATE::Start;
    }
}



void CTestClient::syncCase()
{
    CJsonT msg;
    CJsonTArray ja;
    
    typedef decltype(m_testCase->m_atk) DC;
    DC* pV;
    if (isAttacking())
    {
        pV = &m_testCase->m_atk;
        MT->printA("attack side sync: ");
    }
    else
    {
        pV = &m_testCase->m_def;
        MT->printD("defend side sync: ");
    }
    
    for (int i = 0; i < 11; ++i)
    {
        DC::value_type tpi = nullptr;
        for (auto it = pV->begin(); it != pV->end(); ++it)
        {
            if ((*it)->playerNumber == i)
            {
                tpi = (*it);
                break;
            }
        }
        
        switch (m_side)
        {
            case FBDefs::SIDE::LEFT:
                if (tpi)
                {
                    ja.append(tpi->position.x);
                    ja.append(tpi->position.y);
                    if (isAttacking())
                    {
                        MT->printA("x: %f, y: %f", tpi->position.x, tpi->position.y);
                    }
                    else
                    {
                        MT->printD("x: %f, y: %f", tpi->position.x, tpi->position.y);
                    }
                }
                else
                {
                    ja.append(CJsonT((float)0));
                    ja.append(CJsonT((float)0));
                }
                ja.append(CJsonT((float)0));
                ja.append(CJsonT((float)0));
                break;
            case FBDefs::SIDE::RIGHT:
                if (tpi)
                {
                    ja.append(CJsonT(tpi->position.x));
                    ja.append(CJsonT(tpi->position.y));
                    if (m_ballPos != -1)
                    {
                        MT->printA("x: %f, y: %f", tpi->position.x, tpi->position.y);
                    }
                    else
                    {
                        MT->printD("x: %f, y: %f", tpi->position.x, tpi->position.y);
                    }
                }
                else
                {
                    ja.append(CJsonT((float)FBDefs::PITCH_WIDTH-1));
                    ja.append(CJsonT(0));
                }
                ja.append(CJsonT((float)0));
                ja.append(CJsonT((float)0));
                break;
            default:
                break;
        }
        
    }
    
    msg.setChild("teamPos", ja);
    msg.setChild("side", (int)m_side);
    msg.setChild("ballPosPlayerId", m_ballPos);
    msg.setChild("timeStamp", 0);
    m_pomelo->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    
    msg.release();
}



bool CTestClient::isAttacking()
{
    return m_ballPos != -1;
}