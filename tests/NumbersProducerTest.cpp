#include "NumbersProducer.hpp"

#include "NumberProtocolFactory.hpp"

#include <catch2/catch.hpp>
#include <memory>

SCENARIO("Numbers::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        aleatoric::NumberProtocolFactory factory;
        aleatoric::Range producerRange(20, 30);
        aleatoric::NumbersProducer instance(factory.createBasic(1, 10),
                                            producerRange);

        WHEN("An integer number is requested")
        {
            THEN("It should return a number within in the producer range")
            {
                auto number = instance.getIntegerNumber();
                REQUIRE(producerRange.numberIsInRange(number));
            }
        }

        WHEN("A decimal number is requested")
        {
            THEN("It should return a number within in the producer range")
            {
                auto number = instance.getDecimalNumber();
                REQUIRE(producerRange.numberIsInRange(number));
            }
        }

        WHEN("A collection of integers is requested")
        {
            auto set = instance.getIntegerCollection(1000);

            THEN("Should return the correct amount of numbers")
            {
                REQUIRE(set.size() == 1000);
            }

            THEN("All the numbers should be within the producer range")
            {
                for(auto &&i : set) {
                    REQUIRE(producerRange.numberIsInRange(i));
                }
            }
        }

        WHEN("A collection of decimal numbers is requested")
        {
            auto set = instance.getDecimalCollection(1000);

            THEN("Should return the correct amount of numbers")
            {
                REQUIRE(set.size() == 1000);
            }

            THEN("All the numbers should be within the producer range")
            {
                for(auto &&i : set) {
                    REQUIRE(producerRange.numberIsInRange(i));
                }
            }
        }

        // TODO: DYNAMIC-PARAMS: bin this when reset() is removed or reinstate
        // WHEN("A reset is made")
        // {
        //     THEN("It should call the given protocol reset method")
        //     {
        //         REQUIRE_CALL(*protocolPointer, reset());
        //         instance.reset();
        //     }
        // }
    }
}
