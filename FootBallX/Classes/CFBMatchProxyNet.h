//
//  CFBMatchProxyNet.h
//  FootBallX
//
//  Created by 马 俊 on 14-4-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#ifndef __FootBallX__CFBMatchProxyNet__
#define __FootBallX__CFBMatchProxyNet__

#include "CFBMatchProxy.h"
#include "CSyncedTime.h"

/* 
网络同步几个原则：
 1. 所有指令发起由进攻方客户端决定，比如主动传球，遭遇检测等。服务器不负责遭遇检测之类主动触发事件的发起。
 2. 指令的成功与否是由服务器计算决定的。
*/

class CFBMatchProxyNet : public CFBMatchProxy
{
public:
    CFBMatchProxyNet();
    virtual ~CFBMatchProxyNet();
    
    virtual void start() override;
    virtual void sendTeamPosition(const vector<float>&, int) override;
    
    virtual void setTeamPositionAck(TEAM_POSITION_FUNC) override;
    virtual void setStartMatchAck(START_MATCH_FUNC) override;
    virtual void setEndMatchAck(END_MATCH_FUNC) override;
    
    virtual void update(float dt) override;
    
protected:
    enum class SYNC_TYPE
    {
        ALL_TEAM,
    };
    
    enum class START_STEP
    {
        SYNC_TIME_BEGIN,
        SYNC_TIME,
        SYNC_TIME_END,
        NONE,
    };
    
    void onSync(Node*, void*);
    void onStartMatch(Node*, void*);
    void onEndMatch(Node*, void*);
    void onSwicthDominator(Node*, void*);
    
    TEAM_POSITION_FUNC m_teamPositionAck;
    START_MATCH_FUNC m_startMatchAck;
    END_MATCH_FUNC m_endMatchAck;
    
    CSyncedTime m_syncedTimer;
    
    START_STEP m_startStep = START_STEP::NONE;
    
    bool m_isDominator = false;
};

#endif /* defined(__FootBallX__CFBMatchProxyNet__) */
