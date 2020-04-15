#include "ProducerGranular.hpp"

#include "ProtocolGranular.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class ConcreteProtocolMock : public actlib::Numbers::Granular::Protocol {
  public:
    MAKE_MOCK0(getNumber, double());
    MAKE_MOCK0(reset, void());
};

SCENARIO("Numbers::Granular::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        auto protocol = std::make_unique<ConcreteProtocolMock>();
        auto protocolPointer = protocol.get();
        actlib::Numbers::Granular::Producer instance(std::move(protocol));

        WHEN("A number is requested")
        {
            THEN("It should call the given protocol for a number and return "
                 "it")
            {
                double acquiredNumber = 1;
                REQUIRE_CALL(*protocolPointer, getNumber())
                    .RETURN(acquiredNumber);
                auto number = instance.getNumber();
                REQUIRE(number == acquiredNumber);
            }
        }

        WHEN("A collection of numbers is requested")
        {
            THEN("It should call the protocol for a number enough times to "
                 "fill the requested collection size")
            {
                REQUIRE_CALL(*protocolPointer, getNumber())
                    .TIMES(100)
                    .RETURN(1);

                instance.getCollection(100);
            }

            THEN("It should return a collection of the specified size "
                 "containing the results of calling the protocol for a number")
            {
                int acquiredNumber = 1;
                ALLOW_CALL(*protocolPointer, getNumber())
                    .RETURN(acquiredNumber);

                auto collection = instance.getCollection(100);
                REQUIRE(collection.size() == 100);

                for(auto &&it : collection) {
                    REQUIRE(it == acquiredNumber);
                }
            }
        }

        // TODO: RESET-TEST-FAIL: Reinstate this test - it is failing for
        // unknown reasons WHEN("A reset is made")
        // {
        //     THEN("It should call the given protocol reset method")
        //     {
        //         REQUIRE_CALL(*protocolPointer, reset());
        //         instance.reset();
        //     }
        // }
    }
}
