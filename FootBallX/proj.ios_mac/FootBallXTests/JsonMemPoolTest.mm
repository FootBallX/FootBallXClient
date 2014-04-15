//
//  FootBallMathTest.m
//  FootBallX
//
//  Created by 马 俊 on 14-2-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//
#import <SenTestingKit/SenTestingKit.h>
#include "JsonMemory.h"

@interface JsonMemPoolTest : SenTestCase

@end

@implementation JsonMemPoolTest
- (void)setUp
{
    [super setUp];
    // Put setup code here; it will be run once, before the first test case.
}

- (void)tearDown
{
    // Put teardown code here; it will be run once, after the last test case.
    [super tearDown];
}


- (void)testMath1
{
    CJsonMemPool mem;

    for (int i = 0; i < 319; ++i)
    {
        void* p = mem.pool_alloc(32);
        if ((int)p == 0)
        {
            STFail(@"");
        }
        else
        {
            memset(p, 0, 32);
        }
    }
    
    void* p = mem.pool_alloc(32);
    STAssertTrue(0 != (int)p, @"");
    
    void* p1 = mem.pool_alloc(32);
    STAssertEquals(0, (int)p1, @"");
    
    mem.pool_free(p);
    p = mem.pool_alloc(512);
    STAssertTrue(0 != (int)p, @"");
}


- (void)testMath2
{
}
@end
