//
//  CTestClient.h
//  FBXMatchServerTester
//
//  Created by 马 俊 on 14-5-30.
//
//

#ifndef __FBXMatchServerTester__CTestClient__
#define __FBXMatchServerTester__CTestClient__

#include "Common.h"
#include "CPlayerInfo.h"
#include "FBDefs.h"
#include "CCPomelo.h"

USING_NS_CC;

class CTestCase;

class CTestClient
{
public:
    enum class STATE
    {
        LoginReady,
        SignedUp,
        Paired,
        PrepareMatch,
        Running,
        None,
    };
    
    enum class CASE_STATE
    {
        Start,
        WaitEncounter,
        Encounter,
        None,
    };
    
    CTestClient() = default;
    virtual ~CTestClient();
    
    virtual bool init(const char* username, const char* password);
    virtual bool isLoginReady() const { return m_state == STATE::LoginReady; }
    virtual bool isPaired() const { return m_state == STATE::Paired; }
    virtual bool isRunning() const { return m_state == STATE::Running; }
    virtual bool isCaseDone() const { return isRunning() && m_caseState == CASE_STATE::None; }
    virtual void signUp();
    virtual void getMatchInfo();
    virtual void run(float dt);
    
    virtual void startCase(CTestCase* tc);
protected:
    virtual void connectToConnector(const char* ip, int port);
    virtual void getPlayerInfo(void);
    virtual void onPair(Node* node, void* resp);
    virtual void onStartMatch(Node* node, void* resp);
    virtual void onGetMatchInfo(Node*, void* r);
    virtual void addAllListener();
    virtual void removeAllListener();
    
    virtual void syncNormal();
    virtual void syncCase();
    
    FBDefs::SIDE m_side = FBDefs::SIDE::NONE;
    CCPomelo* m_pomelo = nullptr;
    CPlayerInfo m_playerInfo;
    
    std::string m_username;
    std::string m_password;
    
    STATE m_state = STATE::None;
    
    int m_ballPos = -1;
    float m_syncTime = FLT_MAX;
    
    CTestCase* m_testCase = nullptr;
    CASE_STATE m_caseState = CASE_STATE::None;
};

#endif /* defined(__FBXMatchServerTester__CTestClient__) */
