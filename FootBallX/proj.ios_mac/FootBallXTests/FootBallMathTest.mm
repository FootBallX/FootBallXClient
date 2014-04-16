//
//  FootBallMathTest.m
//  FootBallX
//
//  Created by 马 俊 on 14-2-11.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//
#import <SenTestingKit/SenTestingKit.h>
#include "FBDefs.h"

@interface FootBallMathTest : SenTestCase

@end

@implementation FootBallMathTest
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
    STAssertTrue(FBDefs::isPointOnTheWay(cocos2d::Point(0, 30), cocos2d::Point(15, 0), cocos2d::Point(3, 3)), @"");
}


- (void)testMath2
{
    STAssertTrue(!FBDefs::isPointOnTheWay(cocos2d::Point(0, 50), cocos2d::Point(55, 0), cocos2d::Point(2.5, 2.5)), @"");
}
@end
