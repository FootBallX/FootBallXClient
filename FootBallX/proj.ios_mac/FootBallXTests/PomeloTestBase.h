//
//  PomeloTestBase.h
//  FootBallX
//
//  Created by 马 俊 on 14-1-9.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#include <string>
#include "CCPomelo.h"

using namespace std;

@interface PomeloTestBase : SenTestCase
{
    string m_addr;
    
    int m_port;
    
    float m_timeOut;
    
    bool m_connected;
    
    struct timeval *_lastUpdate;
    
    bool m_OK;
}

- (float)Delta;
- (void)RunAndCheckTimeOut;

@end
