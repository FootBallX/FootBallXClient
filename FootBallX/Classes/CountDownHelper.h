
#ifndef FootballX_CountDownHelper_
#define FootballX_CountDownHelper_

#include "Common.h"
#if 0

class CountDownHelper : public Ref
{

public:
	static CountDownHelper* create(int time, CCLabelBMFont* label);
    static CountDownHelper* create(int time, CCLabelTTF* label);
    
    static void getCHTime(int time, char* result);
public:
	CountDownHelper();
	virtual ~CountDownHelper();
	virtual bool init(int time, CCNode* label);
    virtual void start();
    virtual void start(cc_timeval start_time);
    virtual void start(CCObject* target, SEL_CallFunc selector);
    virtual void start(cc_timeval start_time, CCObject* target, SEL_CallFunc selector);
    virtual void setCompleteSelector(CCObject* target, SEL_CallFunc selector);
    virtual void end();
    void setTime(int time);
    void setEndTime(int time);
    virtual int surplusTime(cc_timeval start_time);
    int surplusLeftTime(uint32_t end_time);
    
    bool displaySysTime;
    int getTime(){return m_time;};
    cc_timeval getStartTime(){return m_start_time;};
private:
    int m_time;
    struct cc_timeval m_start_time;
    int m_end_time;
    CCNode* m_label;
    
    CCObject* m_listenTarget;
	SEL_CallFunc m_selector;
    char dhmsFormat[64];
    void onClockUpdate(float dt);
};
#endif // if 0
#endif