//
//  CMatchTester.cpp
//  FBXMatchServerTester
//
//  Created by 马 俊 on 14-5-30.
//
//

#include "CMatchTester.h"
#include "CTestCaseManager.h"

IMPLEMENT_SINGLETON(CMatchTester);


bool CMatchTester::init()
{
    do
    {
        BREAK_IF_FAILED(m_client[0].init("test1", "123"));
        BREAK_IF_FAILED(m_client[1].init("test2", "123"));
        
        TC->init();

        Director::getInstance()->getScheduler()->schedule(schedule_selector(CMatchTester::update), this, 0, false);
        return true;
    } while (false);
    
    return false;
}



void CMatchTester::update(float dt)
{
    if (m_client[0].isLoginReady() && m_client[1].isLoginReady())
    {
        m_client[0].signUp();
        m_client[1].signUp();
    }
    else if (m_client[0].isPaired() && m_client[1].isPaired())
    {
        m_client[0].getMatchInfo();
        m_client[1].getMatchInfo();
    }
    else if (m_client[0].isRunning() && m_client[1].isRunning())
    {
        if (m_client[0].isCaseDone() && m_client[1].isCaseDone())
        {
            auto tc = TC->getCase(m_caseIndex);
            if (tc)
            {
                m_client[0].startCase(tc);
                m_client[1].startCase(tc);
                tc->repeatCount--;
                if (tc->repeatCount == 0)
                {
                    log("case: %s, repeat: %d, has done!", tc->name.c_str(), tc->repeatCount);
                    m_caseIndex++;
                }
            }
            else
            {
                m_client[0].close();
                m_client[1].close();
            }
        }
        m_client[0].run(dt);
        m_client[1].run(dt);
    }
}