#include "NumbersProducer.hpp"

#include "NumberProtocolMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>

SCENARIO("Numbers::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        auto protocol = std::make_unique<NumberProtocolMock>();
        auto protocolPointer = protocol.get();
        aleatoric::NumbersProducer instance(std::move(protocol));

        WHEN("An integer number is requested")
        {
            THEN("It should call the given protocol for an integer number and "
                 "return it")
            {
                int acquiredNumber = 1;
                REQUIRE_CALL(*protocolPointer, getIntegerNumber())
                    .RETURN(acquiredNumber);
                auto number = instance.getIntegerNumber();
                REQUIRE(number == acquiredNumber);
            }
        }

        WHEN("A decimal number is requested")
        {
            THEN("It should call the given protocol for a decimal number and "
                 "return it")
            {
                int acquiredNumber = 1;
                REQUIRE_CALL(*protocolPointer, getDecimalNumber())
                    .RETURN(acquiredNumber);
                auto number = instance.getDecimalNumber();
                REQUIRE(number == acquiredNumber);
            }
        }

        WHEN("A collection of integers is requested")
        {
            THEN("It should call the protocol for an integer enough times to "
                 "fill the requested collection size")
            {
                REQUIRE_CALL(*protocolPointer, getIntegerNumber())
                    .TIMES(100)
                    .RETURN(1);

                instance.getIntegerCollection(100);
            }

            THEN(
                "It should return a collection of the specified size "
                "containing the results of calling the protocol for an integer")
            {
                int acquiredNumber = 1;
                ALLOW_CALL(*protocolPointer, getIntegerNumber())
                    .RETURN(acquiredNumber);

                auto collection = instance.getIntegerCollection(100);
                REQUIRE(collection.size() == 100);

                for(auto &&it : collection) {
                    REQUIRE(it == acquiredNumber);
                }
            }
        }

        WHEN("A collection of decimal numbers is requested")
        {
            THEN("It should call the protocol for a decimal number enough "
                 "times to "
                 "fill the requested collection size")
            {
                REQUIRE_CALL(*protocolPointer, getDecimalNumber())
                    .TIMES(100)
                    .RETURN(1);

                instance.getDecimalCollection(100);
            }

            THEN("It should return a collection of the specified size "
                 "containing the results of calling the protocol for a decimal "
                 "number")
            {
                int acquiredNumber = 1;
                ALLOW_CALL(*protocolPointer, getDecimalNumber())
                    .RETURN(acquiredNumber);

                auto collection = instance.getDecimalCollection(100);
                REQUIRE(collection.size() == 100);

                for(auto &&it : collection) {
                    REQUIRE(it == acquiredNumber);
                }
            }
        }

        WHEN("A reset is made")
        {
            THEN("It should call the given protocol reset method")
            {
                REQUIRE_CALL(*protocolPointer, reset());
                instance.reset();
            }
        }
    }
}
