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
    json_t *msg = json_object();
    auto jo = json_integer((json_int_t)SYNC_TYPE::CONTROL_PLAYER);
    json_object_set(msg, "type", jo);
    json_decref(jo);
    jo = json_real(moveVec.x);
    json_object_set(msg, "dirX", jo);
    json_decref(jo);
    jo = json_real(moveVec.y);
    json_object_set(msg, "dirY", jo);
    json_decref(jo);
    jo = json_real(playerPos.x);
    json_object_set(msg, "posX", jo);
    json_decref(jo);
    jo = json_real(playerPos.y);
    json_object_set(msg, "posY", jo);
    json_decref(jo);
    
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
}



void CFBMatchProxyNet::sendTeamPosition(const vector<float>& p)
{
    json_t *msg = json_object();
    json_t* jo = json_integer((json_int_t)SYNC_TYPE::ALL_TEAM);
    json_object_set(msg, "type", jo);
    json_decref(jo);
    
    json_t* ja = json_array();
    for (auto x : p)
    {
        jo = json_real(x);
        json_array_append(ja, jo);
        json_decref(jo);
    }
    json_object_set(msg, "teamPos", ja);
    json_decref(ja);
    
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
}



void CFBMatchProxyNet::sendHiligtPlayer(int playerId)
{
    json_t *msg = json_object();
    json_t* jo = json_integer((json_int_t)SYNC_TYPE::SWITCH_HILIGHT);
    json_object_set(msg, "type", jo);
    json_decref(jo);
    
    jo = json_integer((json_int_t)playerId);
    json_object_set(msg, "playerId", jo);
    json_decref(jo);
    
    POMELO->notify("match.matchHandler.sync", msg, [](Node*, void*){});
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
            json_t *msg = json_object();
            POMELO->request(route, msg, [&](Node* node, void* resp){
            });
            break;
        }
        default:
            break;
    }
}



void CFBMatchProxyNet::onSync(Node*, void* resp)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
    auto& docs = ccpomeloresp->docs;
    
    SYNC_TYPE type = (SYNC_TYPE)json_integer_value(json_object_get(docs, "type"));
    
    switch (type)
    {
        case SYNC_TYPE::CONTROL_PLAYER:
        {
            m_playerMoveAck(
                            Point(json_number_value(json_object_get(docs, "posX")),
                                  json_number_value(json_object_get(docs, "posY"))),
                            Point(json_number_value(json_object_get(docs, "dirX")),
                                  json_number_value(json_object_get(docs, "dirY"))));
            break;
        }
        case SYNC_TYPE::ALL_TEAM:
        {
            vector<float> v;
            json_t* ja = json_object_get(docs, "teamPos");
            auto size = json_array_size(ja);
            for (int i = 0; i < size; ++i)
            {
                v.push_back(json_number_value(json_array_get(ja, i)));
            }
            m_teamPositionAck(v);
            break;
        }
        case SYNC_TYPE::SWITCH_HILIGHT:
        {
            m_switchHilightPlayerAck((int)json_integer_value(json_object_get(docs, "playerId")));
            break;
        }
        default:
            break;
    }
}



void CFBMatchProxyNet::onStartMatch(Node*, void* r)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)r;
    auto docs = ccpomeloresp->docs;
    json_t* code = json_object_get(docs, "code");
    if (200 != json_integer_value(code))
    {
        return;
    }
    
    int u1 = (int)json_integer_value(json_object_get(docs, "left"));
    int u2 = (int)json_integer_value(json_object_get(docs, "right"));
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


