#include "ProducerSteps.hpp"

#include "ProtocolSteps.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class ConcreteProtocolMock : public actlib::Numbers::Steps::Protocol {
  public:
    MAKE_MOCK0(getNumber, int());
    MAKE_MOCK0(reset, void());
};

SCENARIO("Numbers::Steps::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        ConcreteProtocolMock protocol;
        actlib::Numbers::Steps::Producer instance(protocol);

        WHEN("A number is requested")
        {
            THEN("It should call the given protocol for a number and return "
                 "it")
            {
                int acquiredNumber = 1;
                REQUIRE_CALL(protocol, getNumber()).RETURN(acquiredNumber);
                auto number = instance.getNumber();
                REQUIRE(number == acquiredNumber);
            }
        }

        WHEN("A reset is made")
        {
            THEN("It should call the given protocol reset method")
            {
                REQUIRE_CALL(protocol, reset());
                instance.reset();
            }
        }
    }
}
