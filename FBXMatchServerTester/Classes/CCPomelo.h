//
//
//  Created by xudexin on 13-4-17.
//
//

#ifndef __CCPomelo__
#define __CCPomelo__

#include "cocos2d.h"
#include "jansson.h"
#include "pomelo.h"
#include "CJsonT.h"
#include "CSingleton.h"
#include <vector>

USING_NS_CC;

class CCPomeloReponse_;
class CCPomeloEvent_ ;
class CCPomeloNotify_;
class CCPomeloConnect_;


class CCPomeloReponse:public cocos2d::Ref
{
public:
    CCPomeloReponse(){}
    ~CCPomeloReponse(){}
    const char* rout;
    int status;
    json_t *docs;
};



class CCPomelo
: public cocos2d::Ref
{
public:
    int connect(const char* addr, int port);
    
    void asyncConnect(const char* addr, int port, std::function<void(Node*, void*)> f);

    void stop();

    int request(const char*route, json_t *msg, std::function<void(Node*, void*)> f);
    int notify(const char*route, json_t *msg, std::function<void(Node*, void*)> f);
    int addListener(const char* event, std::function<void(Node*, void*)> f);
    void removeListener(const char *event);
    
public:
    CCPomelo();
    virtual ~CCPomelo();
    
    void cleanup();
    
    void cleanupEventContent();
    void cleanupNotifyContent();
    void cleanupRequestContent();
    
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

    bool checkClient(pc_client_t* clt);
    bool checkNotify(pc_notify_t* notify);
    bool checkConnect(pc_connect_t* connect);
    bool checkRequest(pc_request_t* request);
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


class CPomeloManager : public CSingleton<CPomeloManager>
{
public:
    bool init(int count);
    int newPomelo();
    CCPomelo* getObject(int index);
    CCPomelo* getObject(pc_client_t* client);
    CCPomelo* getObject(pc_notify_t* notify);
    CCPomelo* getObject(pc_connect_t* connect);
    CCPomelo* getObject(pc_request_t* request);
protected:
    std::vector<CCPomelo*> m_pomeloObjects;
};

#define POMELO_INIT(__N__)      (CPomeloManager::getInstance()->init(__N__))
#define POMELO_NEW          (CPomeloManager::getInstance()->newPomelo())
#define POMELO(__key__)      (CPomeloManager::getInstance()->getObject(__key__))
#endif /* defined(__CCPomelo__) */
