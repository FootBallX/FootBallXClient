#include "CountDownHelper.h"

#if 0
#include "StringSetting.h"

CountDownHelper::CountDownHelper()
{
    m_label = NULL;
    m_listenTarget = NULL;
    m_selector = NULL;
    m_time = -1;
    m_end_time = -1;
    displaySysTime = false;
}

CountDownHelper::~CountDownHelper()
{
    CCLog("CountDownHelper::~CountDownHelper()");
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
    
    CC_SAFE_RELEASE_NULL(m_label);
    m_listenTarget = NULL;
    m_selector = NULL;
}

void CountDownHelper::getCHTime(int time, char* result)
{
//    CCString* hourStr = StringSetting::Instance()->getStringForKey("tHour",DIC_NAME_COMMON);
//    CCString* minStr = StringSetting::Instance()->getStringForKey("tMin",DIC_NAME_COMMON);
//    CCString* secStr = StringSetting::Instance()->getStringForKey("tSec",DIC_NAME_COMMON);
    
    int hourTime = (int)(time / 3600);
    int minTime = (int)((time % 3600) / 60);
    int secTime = (int)(time % 60);
    
    if(hourTime <= 0)
    {
        CCFormat(result, 128,  "%02d:%02d", minTime, secTime);
    }
    else
    {
        CCFormat(result, 128,  "%d:%02d:%02d", hourTime, minTime, secTime);
    }
    
//    wchar_t value[32] = {0};
//
//    if(hourTime > 0)
//    {
//        wprintf(value, "%d", hourTime);
//        result += *value;
//        result += *hourStr->getCString();
//    }
//    
//    if(minTime > 0)
//    {
//        wprintf(value, "%d", minTime);
//        result += *value;
//        result += *minStr->getCString();
//    }
//    
//    if(secTime > 0)
//    {
//        wprintf(value, "%d", secTime);
//        result += *value;
//        result += *secStr->getCString();
//    }
}

CountDownHelper* CountDownHelper::create(int time, CCLabelBMFont* label)
{
    CountDownHelper* pRet = new CountDownHelper();
	if (pRet && pRet->init(time, label))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

CountDownHelper* CountDownHelper::create(int time, CCLabelTTF* label)
{
    CountDownHelper* pRet = new CountDownHelper();
	if (pRet && pRet->init(time, label))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CountDownHelper::init(int time, CCNode* label)
{
    if(dynamic_cast<CCLabelTTF*>(label) == NULL && dynamic_cast<CCLabelBMFont*>(label) == NULL )
    {
        return false;
    }
    
    m_label = label;
    m_label->retain();
    m_time = time;
	return true;
}

void CountDownHelper::setTime(int time)
{
    m_time = time;
}

void CountDownHelper::setEndTime(int time)
{
    m_end_time = time;
}


void CountDownHelper::start(cocos2d::CCObject *target, SEL_CallFunc selector)
{
    m_listenTarget = target;
	m_selector = selector;
    this->start();
}

void CountDownHelper::start(cocos2d::cc_timeval start_time, cocos2d::CCObject *target, SEL_CallFunc selector)
{
    m_listenTarget = target;
	m_selector = selector;
    this->start(start_time);
}

void CountDownHelper::setCompleteSelector(cocos2d::CCObject *target, SEL_CallFunc selector)
{
    m_listenTarget = target;
	m_selector = selector;
}

void CountDownHelper::start()
{
    CCTime::gettimeofdayCocos2d(&m_start_time, NULL);
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CountDownHelper::onClockUpdate), this, 0.5f, false);
    onClockUpdate(0);
}

void CountDownHelper::start(cocos2d::cc_timeval start_time)
{
    m_start_time = start_time;
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(CountDownHelper::onClockUpdate), this, 0.5f, false);
    onClockUpdate(0);
}

void CountDownHelper::end()
{
    CCDirector::sharedDirector()->getScheduler()->unscheduleAllForTarget(this);
}

int CountDownHelper::surplusTime(cocos2d::cc_timeval start_time)
{
    struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	float deltaTime = (now.tv_sec - (start_time).tv_sec) + (now.tv_usec - (start_time).tv_usec) / 1000000.0f;
    
	int result = m_time - deltaTime;
    return result;
}

int CountDownHelper::surplusLeftTime(uint32_t end_time)
{
    struct cc_timeval now;
	CCTime::gettimeofdayCocos2d(&now, NULL);
	double deltaTime = ((long)end_time - now.tv_sec) - (now.tv_usec) / 1000000.0f;
    deltaTime = MAX(0, deltaTime);
    return deltaTime;
}

void CountDownHelper::onClockUpdate(float dt)
{
    int leftTime = 0;
    if(m_end_time == -1)
    {
        leftTime = surplusTime(m_start_time);
    }
    else
    {
        leftTime = surplusLeftTime(m_end_time);
    }

    if (displaySysTime)
    {
        leftTime = 1;
    }
    
    char m_timeChar[50] = {0};
    
	if(leftTime <= 0)
	{
        CCFormat(m_timeChar, sizeof(m_timeChar)-1,  "00:00");
        this->end();
        if(m_listenTarget)
        {
            (m_listenTarget->*m_selector)();
        }
	}
    else
    {
        int day = (int)(leftTime / (3600*24));
        int hourTime = (int) (leftTime%(3600*24) / 3600);
        int minTime = (int)((leftTime % 3600) / 60);
        int secTime = (int)(leftTime % 60);
        if (day<=0)
        {
            if(hourTime <= 0)
            {
                CCFormat(m_timeChar, sizeof(m_timeChar)-1,  "%02d:%02d", minTime, secTime);
            }
            else
            {
                CCFormat(m_timeChar, sizeof(m_timeChar)-1,  "%d:%02d:%02d", hourTime, minTime, secTime);
            }
        }
        else
        {
            CCString* str = StringSetting::Instance()->getStringForKey("tDay", DIC_NAME_COMMON);
            std::string format = std::string("%d") + str->getCString() + std::string(" %d:%02d:%02d");
            CCFormat(m_timeChar, sizeof(m_timeChar)-1,  format.c_str(), day, hourTime, minTime, secTime);
        }
        
        
        if(displaySysTime)
        {
            time_t now_time = time(NULL);
            tm* t= localtime(&now_time);
            CCFormat(m_timeChar, sizeof(m_timeChar)-1,  "%02d:%02d:%02d", t->tm_hour, t->tm_min, t->tm_sec);
        }
    }
    
    if(dynamic_cast<CCLabelTTF*>(m_label) != NULL)
    {
        dynamic_cast<CCLabelTTF*>(m_label)->setString(m_timeChar);
    }
    else if(dynamic_cast<CCLabelBMFont*>(m_label) != NULL)
    {
        dynamic_cast<CCLabelBMFont*>(m_label)->setString(m_timeChar);
    }
}

#endif