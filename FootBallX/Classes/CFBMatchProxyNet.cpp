//
//  CFBMatchProxyNet.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CFBMatchProxyNet.h"
#include "CCPomelo.h"
#include "CFBMatch.h"
#include "CPlayerInfo.h"


CFBMatchProxyNet::CFBMatchProxyNet()
{
    POMELO->addListener("sync", std::bind(&CFBMatchProxyNet::onSync, this, std::placeholders::_1, std::placeholders::_2));
    POMELO->addListener("startMatch", std::bind(&CFBMatchProxyNet::onStartMatch, this, std::placeholders::_1, std::placeholders::_2));
    POMELO->addListener("endMatch", std::bind(&CFBMatchProxyNet::onEndMatch, this, std::placeholders::_1, std::placeholders::_2));
    POMELO->addListener("triggerMenu", std::bind(&CFBMatchProxyNet::onTriggerMenu, this, std::placeholders::_1, std::placeholders::_2));
    POMELO->addListener("instructions", std::bind(&CFBMatchProxyNet::onInstructionResult, this, std::placeholders::_1, std::placeholders::_2));
}



CFBMatchProxyNet::~CFBMatchProxyNet()
{
    POMELO->removeListener("sync");
    POMELO->removeListener("startMatch");
    POMELO->removeListener("endMatch");
    POMELO->removeListener("triggerMenu");
}



void CFBMatchProxyNet::setDelegator(IMatchProxyDelegator* match)
{
    m_match = match;
}


void CFBMatchProxyNet::start()
{
    m_startStep = START_STEP::SYNC_TIME_BEGIN;
}



void CFBMatchProxyNet::sendTeamPosition(const vector<float>& p, int ballPlayerId, int side)
{
    CJsonT msg;

    CJsonTArray ja;
    for (auto x : p)
    {
        ja.append(CJsonT(x));
    }
    msg.setChild("teamPos", ja);
    msg.setChild("side", side);
    msg.setChild("ballPosPlayerId", ballPlayerId);
    msg.setChild("timeStamp", m_syncedTimer.getTime());
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    
    msg.release();
}



void CFBMatchProxyNet::sendMenuCmd(FBDefs::MENU_ITEMS mi, int playerId)
{
    CJsonT msg;
    msg.setChild("cmd", (unsigned int)mi);
    if (playerId > -1)
    {
        msg.setChild("targetPlayerId", (unsigned int)playerId);
    }
    POMELO->request("match.matchHandler.menuCmd", msg, [this](Node* node, void* resp){
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        CJsonT docs(ccpomeloresp->docs);
        unsigned int countDown = docs.getUInt("countDown");
        m_match->instructionAck(countDown);
    });
    msg.release();
}



void CFBMatchProxyNet::update(float dt)
{
    m_syncedTimer.update(dt);
    
    switch (m_startStep)
    {
        case START_STEP::SYNC_TIME_BEGIN:
        {
            if (m_startSyncTime < 0)
            {
                m_startStep = START_STEP::SYNC_TIME;
                m_syncedTimer.startSyncTime();
            }
            else
            {
                m_startSyncTime -= dt;
            }
            break;
        }
        case START_STEP::SYNC_TIME:
        {
            if (!m_syncedTimer.isSyncing())
            {
                m_startStep = START_STEP::SYNC_TIME_END;
            }
            break;
        }
        case START_STEP::SYNC_TIME_END:
        {
            m_startStep = START_STEP::NONE;
            const char *route = "match.matchHandler.ready";
            CJsonT msg;
            POMELO->notify(route, msg, [](Node* node, void* resp){
            });
            msg.release();
            break;
        }
        default:
            break;
    }
}



void CFBMatchProxyNet::onSync(Node*, void* resp)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;

    CJsonT docs(ccpomeloresp->docs);

    vector<float> v;
    CJsonTArray ja(docs.getChild("teamPos"));
    auto size = ja.size();
    for (int i = 0; i < size; ++i)
    {
        v.push_back(ja.get(i).toFloat());
    }
    
    m_match->teamPositionAck(docs.getInt("side"), v, docs.getInt("ballPosPlayerId"), docs.getUInt("timeStamp"));

}



void CFBMatchProxyNet::onStartMatch(Node*, void* r)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)r;
    CJsonT docs(ccpomeloresp->docs);

    CJsonT left(docs.getChild("left"));
    CJsonT right(docs.getChild("right"));
    
    int u1 = left.getInt("uid");
    int u2 = right.getInt("uid");
    
    FBDefs::SIDE side = FBDefs::SIDE::NONE;
    FBDefs::SIDE kickOffSide = FBDefs::SIDE::LEFT;
    
    if (u1 == PLAYER_INFO->getUID())
    {
        side = FBDefs::SIDE::LEFT;
    }
    else if (u2 == PLAYER_INFO->getUID())
    {
        side = FBDefs::SIDE::RIGHT;
    }
    else
    {
        CC_ASSERT(false);
    }
    
    if ( 1 == docs.getInt("kickOffSide"))
    {
        kickOffSide = FBDefs::SIDE::RIGHT;
    }
    
    unsigned int startTime = docs.getUInt("startTime");

    vector<vector<CFBPlayerInitInfo>> v = {vector<CFBPlayerInitInfo>(), vector<CFBPlayerInitInfo>()};
    auto& leftV = v[0];
    auto& rightV = v[1];
    {
        CJsonTArray ja_teamPos(left.getChild("teamPos"));
        CJsonTArray ja_homePos(left.getChild("homePos"));
        CJsonTArray ja_aiClass(left.getChild("aiClass"));
        CC_ASSERT(ja_teamPos.size() == ja_homePos.size() && ja_teamPos.size() == 2 * ja_aiClass.size());
        
        auto size = ja_aiClass.size();
        
        for (int i = 0; i < size; ++i)
        {

            leftV.push_back(CFBPlayerInitInfo(
                                              ja_teamPos.get(i * 2).toFloat(), ja_teamPos.get(i * 2 + 1).toFloat(),
                                              ja_homePos.get(i * 2).toFloat(), ja_homePos.get(i * 2 + 1).toFloat(),
                                              ja_aiClass.get(i).toInt()
                                              )
                            );
        }
    }
    
    {
        CJsonTArray ja_teamPos(right.getChild("teamPos"));
        CJsonTArray ja_homePos(right.getChild("homePos"));
        CJsonTArray ja_aiClass(right.getChild("aiClass"));
        CC_ASSERT(ja_teamPos.size() == ja_homePos.size() && ja_teamPos.size() == 2 * ja_aiClass.size());

        auto size = ja_aiClass.size();
        for (int i = 0; i < size; ++i)
        {
            rightV.push_back(CFBPlayerInitInfo(
                                              ja_teamPos.get(i * 2).toFloat(), ja_teamPos.get(i * 2 + 1).toFloat(),
                                              ja_homePos.get(i * 2).toFloat(), ja_homePos.get(i * 2 + 1).toFloat(),
                                              ja_aiClass.get(i).toInt()
                                              )
                            );
        }
    }
    
    
    m_match->startMatchAck(v, side, kickOffSide, startTime);
}



void CFBMatchProxyNet::onEndMatch(Node*, void* r)
{
    m_match->endMatchAck();
}



void CFBMatchProxyNet::onTriggerMenu(Node*, void* r)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)r;
    CJsonT docs(ccpomeloresp->docs);
    
    int type = docs.getInt("menuType");
    CC_ASSERT(type >= 0 && type < (unsigned int)FBDefs::MENU_TYPE::NONE);
    
    vector<int> av;
    vector<int> dv;
    {
        CJsonTArray ja(docs.getChild("attackPlayers"));
        CC_ASSERT(ja.size() > 0);
        for (int i = 0; i < ja.size(); ++i)
        {
            av.push_back(ja.get(i).toInt());
        }
    }
    {
        CJsonTArray ja(docs.getChild("defendplayers"));
        CC_ASSERT(ja.size() > 0);
        vector<int> v;
        for (int i = 0; i < ja.size(); ++i)
        {
            dv.push_back(ja.get(i).toInt());
        }
    }
    
    m_match->triggerMenuAck((FBDefs::MENU_TYPE)type, av, dv);
}



void CFBMatchProxyNet::onInstructionResult(Node*, void* r)
{
    CFBInstructionResult& res = m_match->getInstructionResult();
    
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)r;
    CJsonT docs(ccpomeloresp->docs);
    
    CJsonTArray ja(docs.getChild("instructions"));
    for (size_t i = 0; i < ja.size(); ++i)
    {
        auto ins = ja.get(i);
        res.instructions.push_back(
                                   CFBInstructionResult::InsStructure(
                                                                      ins.getInt("side"),
                                                                      ins.getInt("playerNumber"),
                                                                      ins.getInt("ins"),
                                                                      ins.getInt("result")
                                                                      )
                                   );
        
        auto& insStru = res.instructions[i];
        
        CJsonTArray animsJson(ins.getChild("animations"));
        for (size_t j = 0; j < animsJson.size(); ++j)
        {
            auto animObj = animsJson.get(j);
            insStru.animations.push_back(
                                         CFBInstructionResult
                                         ::InsStructure
                                         ::Animation(
                                                     animObj.getInt("animId"),
                                                     animObj.getFloat("delay")
                                                     )
                                         );
        }
    }
    
    res.ballSide = docs.getInt("ballSide");
    res.playerNumber = docs.getInt("playerNumber");
    res.ballPosX = docs.getFloat("ballPosX");
    res.ballPosY = docs.getFloat("ballPosY");
    
    m_match->instructionResultAck();
}



unsigned int CFBMatchProxyNet::getTime()
{
    return m_syncedTimer.getTime();
}



float CFBMatchProxyNet::getDeltaTime(unsigned int time)
{
    return ((float)m_syncedTimer.getTime() - (float)time) / 1000.f;
}
