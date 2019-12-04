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

SCENARIO("DiscreteNumberGenerator")
{

    GIVEN("The class is invoked with the default constructor")
    {
        actlib::NumberGenerators::DiscreteNumberGenerator instance;

        WHEN("A number is requested")
        {
            THEN("It should return 0")
            {
                // NB: This is a pseudo test, in that it is unlikely to be wrong, but is not guaranteed to be right!
                // In order to test this properly it would require understanding how to deterministically seed the RNG
                // or Mock things (which I don't think is possible)
                for (int i = 0; i < 100; i++) {
                    REQUIRE(instance.getNumber() == 0);
                }
            }
        }
    }

    GIVEN("The class is invoked with a weighted discrete distribution")
    {
        actlib::NumberGenerators::DiscreteNumberGenerator instance(std::vector<double> {0.0, 1.0});

        WHEN("A number is requested")
        {
            THEN("It should return the expected number")
            {
                // NB: This is a pseudo test, in that it is unlikely to be wrong, but is not guaranteed to be right!
                // In order to test this properly it would require understanding how to deterministically seed the RNG
                // or Mock things (which I don't think is possible)
                for (int i = 0; i < 100; i++) {
                    REQUIRE(instance.getNumber() == 1);
                }
            }
        }

        WHEN("The distribution is updated and a number is requested")
        {
            THEN("It should return the updated expected number")
            {
                instance.updateDistribution(std::vector<double> {0.0, 0.0, 1.0});
                // NB: This is a pseudo test, in that it is unlikely to be wrong, but is not guaranteed to be right!
                // In order to test this properly it would require understanding how to deterministically seed the RNG
                // or Mock things (which I don't think is possible)
                for (int i = 0; i < 100; i++) {
                    REQUIRE(instance.getNumber() == 2);
                }
            }
        }
    }
}

SCENARIO("NumberGenerators::Serial")
{
    GIVEN("The class is invoked with an inclusive range of 3")
    {
        actlib::NumberGenerators::Serial instance(1, 3);
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
