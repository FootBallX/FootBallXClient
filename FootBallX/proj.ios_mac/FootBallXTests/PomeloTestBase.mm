//
//  PomeloTestBase.m
//  FootBallX
//
//  Created by 马 俊 on 14-1-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#import "PomeloTestBase.h"

@implementation PomeloTestBase

- (void)setUp
{
    [super setUp];
    
    // Set-up code here.
    _lastUpdate = new struct timeval;
    gettimeofday(_lastUpdate, nullptr);
    m_timeOut = 5.f;        // 1 sec.
    
    if (0 == POMELO->connect(m_addr.c_str(), m_port))
    {
        m_connected = true;
    }
    else
    {
        m_connected = false;
    }
    
    m_OK = false;
    
    printf("================================================\n");
}

- (void)tearDown
{
    // Tear-down code here.
    delete POMELO;
    delete _lastUpdate;
    [super tearDown];
}

- (float)Delta
{
    struct timeval now;
    float _deltaTime = 0;
    
    if (gettimeofday(&now, nullptr) != 0)
    {
        CCLOG("error in gettimeofday");
        _deltaTime = 0;
    }
    
    _deltaTime = (now.tv_sec - _lastUpdate->tv_sec) + (now.tv_usec - _lastUpdate->tv_usec) / 1000000.0f;
    _deltaTime = MAX(0, _deltaTime);
    
    *_lastUpdate = now;
    
    return _deltaTime;
}

- (void)RunAndCheckTimeOut
{
    while (m_timeOut > 0 && !m_OK)
    {
        float dt = [self Delta];
        POMELO->dispatchCallbacks(dt);
        m_timeOut -= dt;
        sleep(0);
    }
    
    if (m_timeOut <= 0 && !m_OK)
    {
        STFail(@"Time out");
    }
}

@end
