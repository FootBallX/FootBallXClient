//
//  ProtocolHandler.cpp
//  Ragnarok Battle Online
//
//  Created by xudexin on 13-4-17.
//
//

#include "CCPomelo.h"
#include <errno.h>

IMPLEMENT_SINGLETON(CPomeloManager);

bool CPomeloManager::init(int count)
{
    do
    {
        for (int i = 0; i < count; ++i)
        {
            m_pomeloObjects.push_back(new CCPomelo);
        }
        return true;
    } while (false);
    
    return false;
}



int CPomeloManager::newPomelo()
{
    int idx = (int)m_pomeloObjects.size();
    m_pomeloObjects.push_back(new CCPomelo);
    
    return idx;
}



CCPomelo* CPomeloManager::getObject(int index)
{
    if (index >=0 && index < m_pomeloObjects.size())
    {
        return m_pomeloObjects[index];
    }

    CC_ASSERT(false);
    return nullptr;
}



CCPomelo* CPomeloManager::getObject(pc_client_t* client)
{
    for (auto p : m_pomeloObjects)
    {
        if (p->checkClient(client))
        {
            return p;
        }
    }
	return NULL;
}



CCPomelo* CPomeloManager::getObject(pc_notify_t* notify)
{
    for (auto p : m_pomeloObjects)
    {
        if (p->checkNotify(notify))
        {
            return p;
        }
    }
	return NULL;
}



CCPomelo* CPomeloManager::getObject(pc_connect_t* connect)
{
    for (auto p : m_pomeloObjects)
    {
        if (p->checkConnect(connect))
        {
            return p;
        }
    }
	return NULL;
}



CCPomelo* CPomeloManager::getObject(pc_request_t* request)
{
    for (auto p : m_pomeloObjects)
    {
        if (p->checkRequest(request))
        {
            return p;
        }
    }
	return NULL;
}




class CCPomeloReponse_
{
public:
    CCPomeloReponse_()
    {
        request = NULL;
        docs = NULL;
    }
    ~CCPomeloReponse_()
    {
    }
    int status;
    pc_request_t *request;
    json_t *docs;
};



class CCPomeloEvent_
{
public:
    CCPomeloEvent_()
    {
        docs = NULL;
    }
    ~CCPomeloEvent_()
    {
    }
    int status;
    std::string event;
    json_t *docs;
};



class CCPomeloNotify_
{
public:
    CCPomeloNotify_()
    {
        notify = NULL;
    }
    ~CCPomeloNotify_()
    {
    }
    int status;
    pc_notify_t *notify;
};



class CCPomeloConnect_
{
public:
    CCPomeloConnect_(){
        req = NULL;
    }
    ~CCPomeloConnect_(){
        
    }
    int status;
    pc_connect_t *req;
    std::function<void(Node*, void*)> func;
};


void   cc_pomelo_on_ansync_connect_cb(pc_connect_t* conn_req, int status)
{
    auto pomelo = POMELO(conn_req);
    if (conn_req) {
        pc_connect_req_destroy(conn_req);
    }
    pomelo->connectCallBack(status);
}



void cc_pomelo_on_notify_cb(pc_notify_t *ntf, int status)
{
    auto pomelo = POMELO(ntf);
    
    pomelo->lockNotifyQeueue();
    
    CCPomeloNotify_ *notify = new CCPomeloNotify_;
    notify->notify = ntf;
    notify->status = status;
    
    pomelo->pushNotiyf(notify);
    
    pomelo->unlockNotifyQeueue();
}



void cc_pomelo_on_event_cb(pc_client_t *client, const char *event, void *data)
{
    auto pomelo = POMELO(client);
    
    pomelo->lockEventQeueue();
    
    CCPomeloEvent_ *ev = new CCPomeloEvent_;
    ev->event = event;
    ev->docs = (json_t *)data;
    json_incref(ev->docs);
    
    pomelo->pushEvent(ev);
    
    pomelo->unlockEventQeueue();
    
}



void cc_pomelo_on_request_cb(pc_request_t *request, int status, json_t *docs)
{
    auto pomelo = POMELO(request);
    
    pomelo->lockReponsQeueue();
    
    CCPomeloReponse_ *response = new CCPomeloReponse_;
    response->request = request;
    response->status = status;
    response->docs = docs;
    json_incref(docs);
    
    pomelo->pushReponse(response);
    
    pomelo->unlockReponsQeueue();
    
}



void CCPomelo::dispatchRequest()
{
    lockReponsQeueue();
    CCPomeloReponse_ *response = popReponse();
    if (response)
    {
        std::map<pc_request_t*, std::function<void(Node*, void*)> >::iterator it = request_content.find(response->request);
        if (it != request_content.end())
        {
            CCPomeloReponse resp;
            resp.status = response->status;
            resp.docs = response->docs;
            (*it).second((Node*)this, &resp);
            request_content.erase(it);

        }

        json_decref(response->docs);
        json_decref(response->request->msg);
        pc_request_destroy(response->request);
        delete response;
    }
    unlockReponsQeueue();
}



void CCPomelo::dispatchEvent()
{
    lockEventQeueue();
    CCPomeloEvent_ *event = popEvent();
    if (event)
    {
        std::map<std::string, std::function<void(Node*, void*)> >::iterator it = event_content.find(event->event);
        if (it != event_content.end())
        {
            CCPomeloReponse resp;
            resp.status = event->status;
            resp.docs = event->docs;
            (*it).second((Node*)this, &resp);
        }

        json_decref(event->docs);
        delete event;
    }
    unlockEventQeueue();
}



void CCPomelo::dispatchNotify()
{
    lockNotifyQeueue();
    CCPomeloNotify_ *ntf = popNotify();
    if (ntf)
    {
        std::map<pc_notify_t*, std::function<void(Node*, void*)> >::iterator it = notify_content.find(ntf->notify);
        if (it != notify_content.end())
        {
            CCPomeloReponse resp;
            resp.status = ntf->status;
            resp.docs = NULL;
            (*it).second((Node*)this, &resp);
            notify_content.erase(it);
        }

        json_decref(ntf->notify->msg);
        pc_notify_destroy(ntf->notify);
        delete ntf;
    }
    unlockNotifyQeueue();
}



void CCPomelo::connectCallBack()
{
    if (connect_content)
    {
        CCPomeloReponse resp;
        resp.status = connect_content->status;
        resp.docs = NULL;
        connect_content->func((Node*)this, &resp);
        
        connect_status  = 0;
        desTaskCount();
        delete connect_content;
        connect_content = NULL;
    }
}



void CCPomelo::dispatchCallbacks(float delta)
{
    dispatchNotify();
    dispatchEvent();
    dispatchRequest();
    if (connect_status == 1)
    {
        connectCallBack();
    }
    
    pthread_mutex_lock(&task_count_mutex);
    
    if (task_count == 0)
    {
        Director::getInstance()->getScheduler()->pauseTarget(this);
    }
    
    pthread_mutex_unlock(&task_count_mutex);
    
}



CCPomelo::CCPomelo()
{
    Director::getInstance()->getScheduler()->schedule(schedule_selector(CCPomelo::dispatchCallbacks), this, 0, false);
    Director::getInstance()->getScheduler()->pauseTarget(this);
    client = pc_client_new();
    pthread_mutex_init(&reponse_queue_mutex, NULL);
    pthread_mutex_init(&event_queue_mutex, NULL);
    pthread_mutex_init(&notify_queue_mutex, NULL);
    pthread_mutex_init(&task_count_mutex, NULL);
    pthread_mutex_init(&connect_mutex, NULL);
    task_count = 0;
    connect_status = 0;
    connect_content = NULL;
}



CCPomelo::~CCPomelo()
{
    Director::getInstance()->getScheduler()->unschedule(schedule_selector(CCPomelo::dispatchCallbacks), this);
}



int CCPomelo::connect(const char* addr,int port)
{
    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);

    client = pc_client_new();

    int ret = pc_client_connect(client, &address);
    if(ret)
    {
        log("pc_client_connect error:%d", errno);
        pc_client_destroy(client);
    }
    return  ret;
}



void CCPomelo::asyncConnect(const char* addr, int port, std::function<void(Node*, void*)> f)
{
    if (!connect_content) {
        connect_status = 0;
        connect_content = new CCPomeloConnect_;
        connect_content->func = f;
    }else{
        CCLOG("can not call again before the first connect callback");
        return ;
    }
    
    struct sockaddr_in address;
    memset(&address, 0, sizeof(struct sockaddr_in));
    address.sin_family = AF_INET;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(addr);
    
    client = pc_client_new();

    pc_connect_t* async = pc_connect_req_new(&address);
    connect_content->req = async;
    int ret = pc_client_connect2(client,async,cc_pomelo_on_ansync_connect_cb);
    if(ret)
    {
        log("pc_client_connect2 error:%d", errno);
        pc_client_destroy(client);
        cc_pomelo_on_ansync_connect_cb(NULL,ret);
    }
}



void CCPomelo::stop()
{
    pc_client_destroy(client);
}



void CCPomelo::cleanup(){
    cleanupEventContent();
    cleanupNotifyContent();
    cleanupRequestContent();
    pthread_mutex_lock(&task_count_mutex);
    pthread_mutex_unlock(&task_count_mutex);
}


void CCPomelo::cleanupEventContent(){
    auto iter = event_content.begin();
    for (;iter != event_content.end();iter++) {
        pc_remove_listener(client, iter->first.c_str(), cc_pomelo_on_event_cb);
    }
    event_content.clear();
}


void CCPomelo::cleanupNotifyContent(){
    notify_content.clear();
}


void CCPomelo::cleanupRequestContent(){
    request_content.clear();
}


int CCPomelo::request(const char*route, json_t *msg, std::function<void(Node*, void*)> f)
{
    pc_request_t *req = pc_request_new();
    request_content[req] = f;
    pc_request(client,req, route, msg, cc_pomelo_on_request_cb);
    return 0;
}



int CCPomelo::notify(const char*route, json_t *msg, std::function<void(Node*, void*)> f)
{
    pc_notify_t *notify = pc_notify_new();
    notify_content[notify] = f;

    pc_notify(client,notify, route, msg, cc_pomelo_on_notify_cb);
    
    return 0;
}


int CCPomelo::addListener(const char* event, std::function<void(Node*, void*)> f)
{
    std::map<std::string, std::function<void(Node*, void*)> >::iterator it = event_content.find(event);
    if (it != event_content.end())
    {
        event_content.erase(it);
    }
    event_content[event] = f;
    return pc_add_listener(client, event, cc_pomelo_on_event_cb);
}



void CCPomelo::removeListener(const char *event){
    event_content.erase(event);
    pc_remove_listener(client, event, cc_pomelo_on_event_cb);
}



void CCPomelo::incTaskCount()
{
    pthread_mutex_lock(&task_count_mutex);
    task_count++;
    pthread_mutex_unlock(&task_count_mutex);
    Director::getInstance()->getScheduler()->resumeTarget(this);
}



void CCPomelo::desTaskCount()
{
    pthread_mutex_lock(&task_count_mutex);
    task_count--;
    pthread_mutex_unlock(&task_count_mutex);
}



void CCPomelo::lockReponsQeueue()
{
    pthread_mutex_lock(&reponse_queue_mutex);
}



void CCPomelo::unlockReponsQeueue()
{
    pthread_mutex_unlock(&reponse_queue_mutex);
}



void CCPomelo::lockEventQeueue()
{
    pthread_mutex_lock(&event_queue_mutex);
}



void CCPomelo::unlockEventQeueue()
{
    pthread_mutex_unlock(&event_queue_mutex);
}



void CCPomelo::lockNotifyQeueue()
{
    pthread_mutex_lock(&notify_queue_mutex);
}



void CCPomelo::unlockNotifyQeueue()
{
    pthread_mutex_unlock(&notify_queue_mutex);
}



void CCPomelo::lockConnectContent()
{
    pthread_mutex_unlock(&connect_mutex);
}



void CCPomelo::unlockConnectContent()
{
    pthread_mutex_unlock(&connect_mutex);
}



void CCPomelo::pushReponse(CCPomeloReponse_*response)
{
    reponse_queue.push(response);
    incTaskCount();
}



void CCPomelo::pushEvent(CCPomeloEvent_* event)
{
    event_queue.push(event);
    incTaskCount();
}



void CCPomelo::pushNotiyf(CCPomeloNotify_*notify)
{
    notify_queue.push(notify);
    incTaskCount();
}



void CCPomelo::connectCallBack(int status)
{
    connect_status = 1;
    connect_content->status = status;
    incTaskCount();

}



CCPomeloReponse_*CCPomelo::popReponse()
{
    if (reponse_queue.size()>0)
    {
        CCPomeloReponse_ *response = reponse_queue.front();
        reponse_queue.pop();
        desTaskCount();
        return  response;
    }
    else
    {
        return  NULL;
    }
}



CCPomeloEvent_*CCPomelo::popEvent()
{
    if (event_queue.size()>0)
    {
        CCPomeloEvent_ *event = event_queue.front();
        event_queue.pop();
        desTaskCount();
        return  event;
    }
    else
    {
        return  NULL;
    }
}



CCPomeloNotify_*CCPomelo::popNotify()
{
    if (notify_queue.size()>0)
    {
        CCPomeloNotify_ *ntf = notify_queue.front();
        notify_queue.pop();
        desTaskCount();
        return  ntf;
    }
    else
    {
        return  NULL;
    }
}




bool CCPomelo::checkClient(pc_client_t* clt)
{
	return clt == this->client;
}



bool CCPomelo::checkNotify(pc_notify_t* notify)
{
    auto it = notify_content.find(notify);
    return it != notify_content.end();
}



bool CCPomelo::checkConnect(pc_connect_t* connect)
{
    if (connect_content)
    {
        return connect_content->req == connect;
    }
    
	return false;
}



bool CCPomelo::checkRequest(pc_request_t* request)
{
    auto it = request_content.find(request);
	return it != request_content.end();
}



