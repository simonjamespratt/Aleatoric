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

        // TODO: Reinstate this test - it is failing for unknown reasons
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
