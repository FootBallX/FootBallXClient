//
//
//  Created by xudexin on 13-4-17.
//
//

#ifndef __CCPomelo__
#define __CCPomelo__

#include "Common.h"
#include "CSingleton.h"

class CCPomeloReponse_;
class CCPomeloEvent_ ;
class CCPomeloNotify_;
class CCPomeloConnect_;


class CCPomeloReponse:public cocos2d::Object
{
public:
    CCPomeloReponse(){}
    ~CCPomeloReponse(){}
    const char* rout;
    int status;
    json_t *docs;
};



class CCPomelo
: public cocos2d::Object
, public CSingleton<CCPomelo>
{
public:
    int connect(const char* addr, int port);
    
    int asyncConnect(const char* addr, int port, std::function<void(Node*, void*)> f);

    void stop();

    int request(const char*route, json_t *msg, std::function<void(Node*, void*)> f);
    int notify(const char*route, json_t *msg, std::function<void(Node*, void*)> f);
    int addListener(const char* event, std::function<void(Node*, void*)> f);
    
public:
    CCPomelo();
    virtual ~CCPomelo();
    
    void dispatchCallbacks(float delta);
    
    
    void lockReponsQeueue();
    void unlockReponsQeueue();
    void lockEventQeueue();
    void unlockEventQeueue();
    void lockNotifyQeueue();
    void unlockNotifyQeueue();
    
    void lockConnectContent();
    void unlockConnectContent();
    
    
    void pushReponse(CCPomeloReponse_*resp);
    void pushEvent(CCPomeloEvent_*ev);
    void pushNotiyf(CCPomeloNotify_*ntf);
    void connectCallBack(int status);

private:
    void incTaskCount();
    void desTaskCount();
    
    CCPomeloReponse_*popReponse();
    CCPomeloEvent_*popEvent();
    CCPomeloNotify_*popNotify();
    
    std::map<pc_notify_t*, std::function<void(Node*, void*)> > notify_content;
    pthread_mutex_t  notify_queue_mutex;
    std::queue<CCPomeloNotify_*> notify_queue;
    
    std::map<std::string, std::function<void(Node*, void*)> > event_content;
    pthread_mutex_t  event_queue_mutex;
    std::queue<CCPomeloEvent_*> event_queue;
    
    std::map<pc_request_t*, std::function<void(Node*, void*)> > request_content;
    pthread_mutex_t  reponse_queue_mutex;
    std::queue<CCPomeloReponse_*> reponse_queue;
    
    
    pthread_mutex_t  connect_mutex;
    CCPomeloConnect_* connect_content;
    
    
    pthread_mutex_t  task_count_mutex;
    void dispatchRequest();
    void dispatchEvent();
    void dispatchNotify();
    void connectCallBack();
    pc_client_t *client;
    int task_count;
    int connect_status;
};


#define POMELO      (CCPomelo::getInstance())
#endif /* defined(__CCPomelo__) */
