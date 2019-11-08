//
//  main.cpp
//  TestACT
//
//  Created by Simon Pratt on 03/11/2019.
//  Copyright © 2019 Simon Pratt. All rights reserved.
//

#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "fakeit.hpp"
//#include "NumberGenerators.hpp"

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


TEST_CASE("RandomNumberWithinRange") {
    Mock<MyClass> mock;
    When(Method(mock, doSomething)).AlwaysReturn(1);
    
    SECTION("Testing") {
        MyClass &instance = mock.get();
        instance.doSomething();
        Verify(Method(mock, doSomething));
        REQUIRE(instance.doSomething() == 1);
    }
    
//    Mock<actlib::RandomNumber> mockRandomNumber;
//    When(Method(mockRandomNumber,getNumber)).Return(1);
//    actlib::RandomNumberWithinRange rnwr(1, 3);
//
//    SECTION("It should produce random numbers within provided the range") {
//        int number = rnwr.getNumber();
//        REQUIRE(number > 0);
//        REQUIRE(number < 4);
//    }
}
