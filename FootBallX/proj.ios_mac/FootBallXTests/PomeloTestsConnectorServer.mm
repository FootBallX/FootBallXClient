//
//  PomeloTestsConnectorServer.m
//  FootBallX
//
//  Created by 马 俊 on 14-1-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#import "PomeloTestsConnectorServer.h"

@implementation PomeloTestsConnectorServer
- (void)setUp
{
    m_addr = "192.168.1.6";
    m_port = 3010;
    
    [super setUp];
    
    // Set-up code here.
    
}

- (void)tearDown
{
    // Tear-down code here.
    [super tearDown];
}





// test开头表示测试用例
// 0001序号表示测试用例执行顺序。默认是按照函数名的字符串顺序执行的。
- (void)test0001_LoginOK
{
    STAssertTrue(m_connected, @"Fail to connect to server");
    const char* route = "connector.entryHandler.login";
    json_t *msg = json_object();
    json_object_set(msg, "userName", json_string("test1"));
    json_object_set(msg, "password", json_string("123"));
    POMELO->request
    (route, msg,
     [&](Node* node, void* resp)
    {
        CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
        json_t* code = json_object_get(ccpomeloresp->docs, "code");
        json_int_t c = json_integer_value(code);
        STAssertTrue(200 == c, @"Fail to login. code: %s", json_dumps(ccpomeloresp->docs, JSON_COMPACT));
        m_OK = true;
    });
    
    [self RunAndCheckTimeOut];
}


- (void)test0002_LoginWithUserNameError
{
//    sleep(1);
    [self test0001_LoginOK];
    gettimeofday(_lastUpdate, nullptr);
    m_timeOut = 5.f;
    m_OK = false;

    const char* route = "connector.entryHandler.getPlayerInfo";
    json_t *msg = json_object();
    POMELO->request
    (route, msg,
     [&](Node* node, void* resp)
    {
         CCPomeloReponse* ccpomeloresp = (CCPomeloReponse*)resp;
         json_t* code = json_object_get(ccpomeloresp->docs, "code");
         json_int_t c = json_integer_value(code);
         STAssertTrue(200 == c, @"Fail to get player info. code: %s", json_dumps(ccpomeloresp->docs, JSON_COMPACT));
         m_OK = true;
    });

    [self RunAndCheckTimeOut];
}

@end
