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
}



CFBMatchProxyNet::~CFBMatchProxyNet()
{
    POMELO->removeListener("sync");
    POMELO->removeListener("startMatch");
    POMELO->removeListener("endMatch");
}



void CFBMatchProxyNet::start()
{
    m_startStep = START_STEP::SYNC_TIME_BEGIN;
}



void CFBMatchProxyNet::sendPlayerMove(const Point& playerPos, const Point& moveVec)
{
    CJsonT msg;
    msg.setChild("type", (json_int_t)SYNC_TYPE::CONTROL_PLAYER);
    msg.setChild("dirX", moveVec.x);
    msg.setChild("dirY", moveVec.y);
    msg.setChild("posX", playerPos.x);
    msg.setChild("posY", playerPos.y);
    
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    
    msg.release();
}



void CFBMatchProxyNet::sendTeamPosition(const vector<float>& p)
{
    CJsonT msg;
    msg.setChild("type", (json_int_t)SYNC_TYPE::ALL_TEAM);
    
    CJsonTArray ja;
    for (auto x : p)
    {
        ja.append(CJsonT(x));
    }
    msg.setChild("teamPos", ja);
    
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    
    msg.release();
}



void CFBMatchProxyNet::sendHiligtPlayer(int playerId)
{
    CJsonT msg;
    msg.setChild("type", (json_int_t)SYNC_TYPE::SWITCH_HILIGHT);
    msg.setChild("playerId", playerId);
    
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
    msg.release();
}



void CFBMatchProxyNet::setPlayerMoveAck(PLAYER_MOVE_FUNC f)
{
    m_playerMoveAck = f;
}



void CFBMatchProxyNet::setTeamPositionAck(TEAM_POSITION_FUNC f)
{
    m_teamPositionAck = f;
}



void CFBMatchProxyNet::setSwitchHilightPlayerAkc(SWITCH_HILIGHT_PLAYER_FUNC f)
{
    m_switchHilightPlayerAck = f;
}



void CFBMatchProxyNet::setStartMatchAck(START_MATCH_FUNC f)
{
    m_startMatchAck = f;
}



void CFBMatchProxyNet::setEndMatchAck(END_MATCH_FUNC f)
{
    m_endMatchAck = f;
}



void CFBMatchProxyNet::update(float dt)
{
    switch (m_startStep)
    {
        case START_STEP::SYNC_TIME_BEGIN:
        {
            m_startStep = START_STEP::SYNC_TIME;
            m_syncedTimer.startSyncTime();
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
            POMELO->request(route, msg, [&](Node* node, void* resp){
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
        case SYNC_TYPE::CONTROL_PLAYER:
        {
            m_playerMoveAck(
                            Point(docs.getFloat("posX"),
                                  docs.getFloat("posY")),
                            Point(docs.getFloat("dirX"),
                                  docs.getFloat("dirY")));
            break;
        }
        case SYNC_TYPE::ALL_TEAM:
        {
            vector<float> v;
            CJsonTArray ja(docs.getChild("teamPos"));
            auto size = ja.size();
            for (int i = 0; i < size; ++i)
            {
                v.push_back(ja.get(i).toFloat());
            }
            m_teamPositionAck(v);
            break;
        }
        case SYNC_TYPE::SWITCH_HILIGHT:
        {
            m_switchHilightPlayerAck(docs.getInt("playerId"));
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

    if (200 != docs.getInt("code"))
    {
        return;
    }
    
    int u1 = docs.getInt("left");
    int u2 = docs.getInt("right");
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
    
    if ( 1 == json_integer_value(json_object_get(docs, "kickOffSide")))
    {
        kickOffSide = FBDefs::SIDE::RIGHT;
    }
    
    long long startTime = json_integer_value(json_object_get(docs, "startTime"));

    m_startMatchAck(side, kickOffSide, startTime);
}



void CFBMatchProxyNet::onEndMatch(Node*, void* r)
{
    m_endMatchAck();
}


