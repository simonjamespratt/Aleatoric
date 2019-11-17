//
//  main.cpp
//  Test
//
//  Created by Simon Pratt on 03/11/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "fakeit.hpp"
#include "NumberGenerators.hpp"

using namespace fakeit;

struct SomeInterface {
    int foo(int a) {return a * 2; }
    int bar(int a, int b) {return a + b; }
};

class MyClass {
public:
    MyClass() {};
    ~MyClass() {};
    virtual int doSomething() {return 5; };
};


TEST_CASE("SomeInterface testing") {
    Mock<MyClass> mock;
    When(Method(mock, doSomething)).AlwaysReturn(1);

    SECTION("Testing") {
        MyClass &instance = mock.get();
        instance.doSomething();
        Verify(Method(mock, doSomething));
        REQUIRE(instance.doSomething() == 1);
    }
}

TEST_CASE("UniformNumberGenerator")
{
    actlib::NumberGenerators::UniformNumberGenerator instance(1, 2);

    SECTION("It should produce random numbers within provided the range")
    {
        // NB: This is a pseudo test, in that it is unlikely to be wrong, but is not guaranteed to be right!
        // In order to test this properly it would require understanding how to deterministically seed the RNG
        // or Mock things (which I don't think is possible)
        for (int i = 0; i < 100; i++) {
            int number = instance.getNumber();
            REQUIRE(number >= 1);
            REQUIRE(number <= 2);
        }
    }
}
