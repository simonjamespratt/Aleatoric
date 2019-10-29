//
//  TestACTLib.m
//  TestACTLib
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "helloWorld.hpp"


@interface helloWorld : XCTestCase

@end

@implementation helloWorld

- (void)testAddStuff
{
    XCTAssert(actlib::utilities::addStuff(1, 1) == 2);
}

- (void)testGetHelloWorld {
    XCTAssert(actlib::utilities::getHelloWorld() == "Hello world!");
}

@end
