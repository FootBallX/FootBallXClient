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


bool CMatchTester::startAll()
{
    do
    {
        BREAK_IF_FAILED(m_client[0].init("test1", "123"));
        BREAK_IF_FAILED(m_client[1].init("test2", "123"));

        Director::getInstance()->getScheduler()->schedule(schedule_selector(CMatchTester::updateAll), this, 0, false);
        return true;
    } while (false);
    
    return false;
}



void CMatchTester::updateAll(float dt)
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
                    print("case: %s, repeat: %d, has done!", tc->name.c_str(), tc->repeatCount);
                    m_caseIndex++;
                }
            }
            else
            {
                m_client[0].close();
                m_client[1].close();
                
                Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
            }
        }
        m_client[0].run(dt);
        m_client[1].run(dt);
    }
}


bool CMatchTester::startCase(int index)
{
    do
    {
        BREAK_IF_FAILED(m_client[0].init("test1", "123"));
        BREAK_IF_FAILED(m_client[1].init("test2", "123"));
        
        m_caseIndex = index;
        
        Director::getInstance()->getScheduler()->schedule(schedule_selector(CMatchTester::update), this, 0, false);
        return true;
    } while (false);
    
    return false;
}



void CMatchTester::update(float dt)
{
    if (m_client[0].isLoginReady() && m_client[1].isLoginReady())
    {
        print("start sign up");
        m_client[0].signUp();
        m_client[1].signUp();
    }
    else if (m_client[0].isPaired() && m_client[1].isPaired())
    {
        print("paired!");
        print("get match info start");
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
                print("case %s, repeat: %d, started!", tc->name.c_str(), tc->repeatCount);
                m_client[0].startCase(tc);
                m_client[1].startCase(tc);
                tc->repeatCount--;
                if (tc->repeatCount == 0)
                {
                    print("case: %s, repeat: %d, has done!", tc->name.c_str(), tc->repeatCount);
                    m_caseIndex = -1;
                }
            }
            else
            {
                Director::getInstance()->getScheduler()->unscheduleAllForTarget(this);
                m_client[0].close();
                m_client[1].close();
            }
        }
        m_client[0].run(dt);
        m_client[1].run(dt);
    }
}



void CMatchTester::print(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    static char buf[MAX_LOG_LENGTH];
    vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
    va_end(args);
    
    if (m_logFunc)
    {
        m_logFunc(buf, Color3B::GREEN);
    }
    else
    {
        CCLOG("%s", buf);
    }
}


void CMatchTester::printD(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    static char buf[MAX_LOG_LENGTH];
    vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
    va_end(args);
    
    if (m_logFunc)
    {
        m_logFunc(buf, Color3B::BLUE);
    }
    else
    {
        CCLOG("%s", buf);
    }
}



void CMatchTester::printA(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    static char buf[MAX_LOG_LENGTH];
    vsnprintf(buf, MAX_LOG_LENGTH-3, format, args);
    va_end(args);
    
    if (m_logFunc)
    {
        m_logFunc(buf, Color3B::RED);
    }
    else
    {
        CCLOG("%s", buf);
    }
}