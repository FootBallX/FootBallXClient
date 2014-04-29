//
//  CSyncedTime.cpp
//  FootBallX
//
//  Created by 马 俊 on 14-4-13.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#include "CSyncedTime.h"


bool CSyncedTime::init()
{
    do
    {
        return true;
    } while (false);
    
	return false;
}



void CSyncedTime::update(float dt)
{
    float delta = dt * 1000.f;
    m_serverTime += (int)delta;
    
    if (m_isSyncing && m_startSyncTime < 0)
    {
        syncTime();
        m_startSyncTime = FLT_MAX;
    }
    
    m_startSyncTime -= dt;
}



void CSyncedTime::startSyncTime()
{
    m_isSyncing = true;
    m_syncCount = m_syncCountMax;
    m_pings.clear();
    
    m_startSyncTime = 0.f;
}



void CSyncedTime::syncTime()
{
    CJsonT msg;
    msg.setChild("cTime", getClientTime());
    POMELO->request("match.matchHandler.time", msg, bind(&CSyncedTime::syncTimeAck, this, std::placeholders::_1, std::placeholders::_2));
    msg.release();
}



void CSyncedTime::syncTimeAck(Node*, void* resp)
{
    CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
    CJsonT docs(ccpomeloresp->docs);
    m_serverTime = docs.getJsonInt("sTime");
    auto lct = docs.getJsonInt("cTime");
    auto ct = getClientTime();
    int ping = (ct - lct) * 0.5;
    m_pings.push_back(ping);
    
    m_syncCount--;
    
    if (m_syncCount > 0)
    {
        log("serT: %lld", m_serverTime);
        log("ping %d -- > %d", m_syncCount, ping);
        m_startSyncTime = 0.25f;
    }
    else
    {
        m_isSyncing = false;
        m_serverTime += std::accumulate(m_pings.begin(), m_pings.end(), 0) / m_pings.size();
    }
}



long long CSyncedTime::getClientTime()
{
    struct timeval now;
    
    gettimeofday(&now, nullptr);
    
    return now.tv_sec * 1000  + now.tv_usec / 1000;
}



long long CSyncedTime::getTime()
{
    return m_serverTime;
}



bool CSyncedTime::isSyncing()
{
    return m_isSyncing;
}


