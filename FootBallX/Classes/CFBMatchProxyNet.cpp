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
}



CFBMatchProxyNet::~CFBMatchProxyNet()
{
    POMELO->removeListener("sync");
    POMELO->removeListener("startMatch");
    POMELO->removeListener("endMatch");
    POMELO->removeListener("triggerMenu");
}



void CFBMatchProxyNet::start()
{
    m_startStep = START_STEP::SYNC_TIME_BEGIN;
}



void CFBMatchProxyNet::sendTeamPosition(const vector<float>& p, int ballPlayerId)
{
    CJsonT msg;
    msg.setChild("type", (json_int_t)SYNC_TYPE::ALL_TEAM);
    
    CJsonTArray ja;
    for (auto x : p)
    {
        ja.append(CJsonT(x));
    }
    msg.setChild("teamPos", ja);
    msg.setChild("ballPosPlayerId", CJsonT(ballPlayerId));
    msg.setChild("timeStamp", CJsonT(m_syncedTimer.getTime()));
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    
    msg.release();
}



void CFBMatchProxyNet::sendMenuCmd(const vector<FBDefs::MENU_ITEMS>& mi)
{
    CJsonT msg;
    CJsonTArray ja;
    for (auto x : mi)
    {
        ja.append(CJsonT((unsigned int)x));
    }
    msg.setChild("cmds", ja);
    POMELO->notify("match.matchHandler.menuCmd", msg, [](Node*, void*){});
    msg.release();
}



void CFBMatchProxyNet::setTeamPositionAck(TEAM_POSITION_FUNC f)
{
    m_teamPositionAck = f;
}



void CFBMatchProxyNet::setStartMatchAck(START_MATCH_FUNC f)
{
    m_startMatchAck = f;
}



void CFBMatchProxyNet::setEndMatchAck(END_MATCH_FUNC f)
{
    m_endMatchAck = f;
}



void CFBMatchProxyNet::setTriggerMenuAck(TRIGGER_MENU_FUNC f)
{
    m_triggerMenuAck = f;
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
            POMELO->request(route, msg, [](Node* node, void* resp){
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

    switch ((SYNC_TYPE)docs.getInt("type"))
    {
        case SYNC_TYPE::ALL_TEAM:
        {
            vector<float> v;
            CJsonTArray ja(docs.getChild("teamPos"));
            auto size = ja.size();
            for (int i = 0; i < size; ++i)
            {
                v.push_back(ja.get(i).toFloat());
            }
            
            m_teamPositionAck(v, docs.getInt("ballPosPlayerId"), docs.getUInt("timeStamp"));
            break;
        }
        default:
            break;
    }
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

    vector<vector<float>> v = {vector<float>(), vector<float>()};
    vector<float>& leftV = v[0];
    vector<float>& rightV = v[1];
    {
        CJsonTArray ja(left.getChild("teamPos"));
        auto size = ja.size();
        for (int i = 0; i < size; ++i)
        {
            leftV.push_back(ja.get(i).toFloat());
        }
    }
    
    {
        CJsonTArray ja(right.getChild("teamPos"));
        auto size = ja.size();
        for (int i = 0; i < size; ++i)
        {
            rightV.push_back(ja.get(i).toFloat());
        }
    }
    
    
    m_startMatchAck(v, side, kickOffSide, startTime);
}



void CFBMatchProxyNet::onEndMatch(Node*, void* r)
{
    m_endMatchAck();
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
    
    m_triggerMenuAck((FBDefs::MENU_TYPE)type, av, dv);
}



unsigned int CFBMatchProxyNet::getTime()
{
    return m_syncedTimer.getTime();
}



float CFBMatchProxyNet::getDeltaTime(unsigned int time)
{
    return (m_syncedTimer.getTime() - time) / 1000.f;
}
