//
//  NumberGenerators.mm
//  TestACTLib
//
//  Created by Simon Pratt on 29/10/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#import <XCTest/XCTest.h>
#include "NumberGenerators.hpp"


@interface NumberGenerators : XCTestCase

@end

@implementation NumberGenerators

- (void)testAddTwoNumbers
{
    XCTAssert(actlib::NumberGenerators::addTwoNumbers(1, 1) == 2);
}

@end
