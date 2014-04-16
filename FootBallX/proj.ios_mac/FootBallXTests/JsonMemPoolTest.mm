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


- (void)testJsonMemPool1
{
    std::vector<int> v = {128,  64,  64,  32,  32};
    CJsonMemPool mem(v);

    int totalSize = std::accumulate(v.begin(), v.end(), 0);

    int buffSize = 32;
    int j = 0;
    int count = v[j];
    for (int i = 0; i < totalSize-1; ++i)
    {
        if (i >= count)
        {
            ++j;
            count += v[j];
            buffSize *= 2;
        }
        
        void* p = mem.pool_alloc(buffSize);
        if ((int)p == 0)
        {
            STFail(@"");
        }
        else
        {
            memset(p, 0xcc, buffSize);
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


- (void)testJsonMemPool2
{

    std::vector<int> v = {1,  1,  1,  1, 1};
    CJsonMemPool mem(v);
    
    auto p1 = mem.pool_alloc(32);
    auto p2 = mem.pool_alloc(64);
    auto p3 = mem.pool_alloc(128);
    auto p4 = mem.pool_alloc(256);
    auto p5 = mem.pool_alloc(512);
    
    memset(p1, 0, 32);
    memset(p2, 0, 64);
    memset(p3, 0, 128);
    memset(p4, 0, 256);
    memset(p5, 0, 512);
    
    STAssertTrue(mem.checkSize(p1, 32), @"");
    STAssertTrue(mem.checkSize(p2, 64), @"");
    STAssertTrue(mem.checkSize(p3, 128), @"");
    STAssertTrue(mem.checkSize(p4, 256), @"");
    STAssertTrue(mem.checkSize(p5, 512), @"");
    
    void* p = mem.pool_alloc(32);
    STAssertEquals(0, (int)p, @"");
}



- (void)testJsonMemPoolTooBigBuffer
{
    std::vector<int> v = {1,  1,  1,  1, 1};
    CJsonMemPool mem(v);
    
    void* p = mem.pool_alloc(1024);
    STAssertEquals(0, (int)p, @"");
}



- (void)testJsonMemPoolMiniSize
{
    std::vector<int> v = {1};
    CJsonMemPool mem(v);
    
    for (int i = 0 ; i <= 32; ++i)
    {
        void* p = mem.pool_alloc(i);
        STAssertTrue(0 != (int)p, @"");
        STAssertTrue(mem.checkSize(p, i), @"");
        mem.pool_free(p);
    }
    
    void* p = mem.pool_alloc(33);
    STAssertTrue(0 == (int)p, @"");
}


- (void)testJsonMemPoolGapSize
{
    std::vector<int> v = {0, 0, 0, 0, 0, 1};
    CJsonMemPool mem(v);
    
    for (int i = 0 ; i <= 1024; ++i)
    {
        void* p = mem.pool_alloc(i);
        STAssertTrue(0 != (int)p, @"");
        STAssertTrue(mem.checkSize(p, i), @"");
        mem.pool_free(p);
    }
    
    void* p = mem.pool_alloc(1025);
    STAssertTrue(0 == (int)p, @"");
}

@end
