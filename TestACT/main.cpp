//
//  main.cpp
//  TestACT
//
//  Created by Simon Pratt on 03/11/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "NumberGenerators.hpp"

TEST_CASE("RandomNumberWithinRange") {
    actlib::NumberGenerators::RandomNumberWithinRange rnwr(1, 3);
    
    SECTION("It should produce random numbers within provided the range") {
        int number = rnwr.getNumber();
        REQUIRE(number > 0);
        REQUIRE(number < 4);
    }
}
