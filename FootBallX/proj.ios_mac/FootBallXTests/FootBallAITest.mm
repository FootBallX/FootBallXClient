//
//  FootBallAITest.m
//  FootBallX
//
//  Created by 马 俊 on 14-1-22.
//  Copyright (c) 2014年 马 俊. All rights reserved.
//

#import <SenTestingKit/SenTestingKit.h>
#include "CFBMatch.h"
#include "CFBTeam.h"

@interface FootBallAITest : SenTestCase

@end

@implementation FootBallAITest

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

- (void)testStartMatch
{
    STAssertTrue(FBMATCH->init(), @"Fail to init match");
    
    CFBTeam* red = new CFBTeam;
    STAssertTrue(red->init(), @"init red fail");
    CFBTeam* black = new CFBTeam;
    STAssertTrue(black->init(), @"init red fail");
    FBMATCH->setRedTeam(red);
    FBMATCH->setBlackTeam(black);
    STAssertTrue(FBMATCH->startMatch(), @"Fail to start match");
}

@end
