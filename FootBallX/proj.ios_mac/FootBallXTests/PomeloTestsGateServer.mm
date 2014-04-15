//
//  PomeloTests.m
//  FootBallX
//
//  Created by 马 俊 on 14-1-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#import "PomeloTestsGateServer.h"


@implementation PomeloTestsGateServer

- (void)setUp
{
    m_addr = "192.168.1.5";
    m_port = 3017;
    
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
- (void)test0001_QueryConnectors
{
    STAssertTrue(m_connected, @"Fail to connect to server");
    const char *route = "gate.gateHandler.queryConnectorEntry";
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
        STAssertTrue(200 == c, @"Fail to query connector entry. code: %s", json_dumps(ccpomeloresp->docs, JSON_COMPACT));
        m_OK = true;
    });
    
    [self RunAndCheckTimeOut];
}


@end
