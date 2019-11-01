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

- (void)testRandomNumberWithinRange
{
    // It should produce random numbers within provided the range
    actlib::NumberGenerators::RandomNumberWithinRange rnwr(1, 3);
    for (int i = 0; i < 10; i++) {
        auto number = rnwr.getNumber();
        XCTAssert(number > 0);
        XCTAssert(number < 4);
    }
}

@end
