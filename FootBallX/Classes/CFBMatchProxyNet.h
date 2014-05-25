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
    
    virtual void setDelegator(IMatchProxyDelegator*) override;
    virtual void start() override;
    virtual void sendTeamPosition(const vector<float>&, int, int) override;
    virtual void sendMenuCmd(FBDefs::MENU_ITEMS, int) override;
    virtual void sendInstructionMovieEnd() override;

    virtual void update(float dt) override;
    
    virtual unsigned int getTime() override;
    virtual float getDeltaTime(unsigned int) override;
protected:    
    enum class START_STEP
    {
        SYNC_TIME_BEGIN,
        SYNC_TIME,
        SYNC_TIME_END,
        WAITING_MATCH_INFO,
        NONE,
    };
    
    void onSync(Node*, void*);
    void onStartMatch(Node*, void*);
    void onEndMatch(Node*, void*);
    void onTriggerMenu(Node*, void*);
    void onInstructionResult(Node*, void*);
    void onGetMatchInfo(Node*, void*);
    void onInstructionDone(Node*, void*);
    void onResumeMatch(Node*, void*);
    
    CSyncedTime m_syncedTimer;
    
    START_STEP m_startStep = START_STEP::NONE;
    
    float m_startSyncTime = 2.0f;
    
    IMatchProxyDelegator* m_match = nullptr;
};

#endif /* defined(__FootBallX__CFBMatchProxyNet__) */
