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
        ConcreteProtocolMock protocol;
        actlib::Numbers::Granular::Producer instance(protocol);

        WHEN("A number is requested")
        {
            THEN("It should call the given protocol for a number and return "
                 "it")
            {
                double acquiredNumber = 1;
                REQUIRE_CALL(protocol, getNumber()).RETURN(acquiredNumber);
                auto number = instance.getNumber();
                REQUIRE(number == acquiredNumber);
            }
        }
    }
}
