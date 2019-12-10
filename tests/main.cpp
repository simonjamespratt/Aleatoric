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
#include "Engine.hpp"
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

SCENARIO("Numbers::Serial")
{
    GIVEN("The class is invoked with an inclusive range of 3")
    {
        // TODO: This needs to be instantiated ONCE for ALL tests to use,
        // ideally in the main.cpp of the tests
        // It also needs to be handled by a smart pointer or Engine::Instance() needs to return a smart pointer
        actlib::RNG::Engine *engine = actlib::RNG::Engine::Instance();
        actlib::NumberGenerators::Serial instance(engine->getEngine(), 1, 3);
        int firstNumber = instance.getNumber();
        int secondNumber = instance.getNumber();
        int thirdNumber = instance.getNumber();
        int fourthNumber = instance.getNumber();

        WHEN("The first number is requested")
        {
            THEN("The number returned should be within the range specified")
            {
                REQUIRE(firstNumber >= 1);
                REQUIRE(firstNumber <=3);
            }
        }

        WHEN("The second number is requested")
        {
            THEN("The number returned should be any within the range specified")
            {
                REQUIRE(secondNumber >= 1);
                REQUIRE(secondNumber <=3);

            }

            THEN("The number returned should not be the first number requested")
            {
                REQUIRE_FALSE(secondNumber == firstNumber);
            }
        }

        WHEN("The third number is requested")
        {
            THEN("The number returned should be any within the range specified")
            {
                REQUIRE(thirdNumber >= 1);
                REQUIRE(thirdNumber <=3);

            }

            THEN("The number returned should not be the first or second number requested")
            {
                REQUIRE_FALSE(thirdNumber == firstNumber);
                REQUIRE_FALSE(thirdNumber == secondNumber);
            }
        }

        WHEN("The fourth number is requested")
        {
            THEN("The number returned should be any within the range specified")
            {
                REQUIRE(fourthNumber >= 1);
                REQUIRE(fourthNumber <=3);

            }

            THEN("The probability should have reset to be equal and number returned should be one of the previously selected numbers")
            {
                std::array<int, 3> previouslySelectedNumbers = {firstNumber, secondNumber, thirdNumber};
                bool result = std::any_of(previouslySelectedNumbers.begin(), previouslySelectedNumbers.end(), [fourthNumber](int i){return i == fourthNumber;});
                REQUIRE(result == true);
            }
        }
    }
}
