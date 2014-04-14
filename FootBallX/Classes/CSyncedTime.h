//
//  CSyncedTime.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-13.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CSyncedTime__
#define __FootBallX__CSyncedTime__

#include "Common.h"
#include "CCPomelo.h"
#include "CSingleton.h"

class CSyncedTime
{
public:
    CSyncedTime() = default;
    ~CSyncedTime() = default;
    
    bool init();
    void update(float dt);
    void startSyncTime();
    bool isSyncing();
    long long getTime();
protected:
    void syncTime();
    void syncTimeAck(Node*, void*);
    long long getClientTime();
    
    long long m_serverTime = 0;
    
    bool m_isSyncing = false;
    const int m_syncCountMax = 5;
    int m_syncCount;
    
    vector<int> m_pings;
};


#endif /* defined(__FootBallX__CSyncedTime__) */
