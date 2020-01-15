#include "NoRepetition.hpp"

#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::NoRepetition")
{
    GIVEN("The class is instantiated correctly")
    {
        actlib::Numbers::Steps::Range range(1, 3);
        DiscreteGeneratorMock generator;
        int generatedNumber = 1;
        ALLOW_CALL(generator, updateDistributionVector(1.0));
        ALLOW_CALL(generator, updateDistributionVector(generatedNumber, 0.0));
        actlib::Numbers::Steps::NoRepetition instance(generator, range);

        WHEN("A number is requested")
        {
            THEN("It should request a number from the generator")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                instance.getNumber();
            }

            THEN("It should return the generated number with the range offset "
                 "applied")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            THEN("It should ensure that the generated number cannot be "
                 "selected at the next time of asking whilst ensuring all "
                 "others have "
                 "equal probability of being selected")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                REQUIRE_CALL(generator, updateDistributionVector(1.0));
                REQUIRE_CALL(generator,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getNumber();
            }
        }

        WHEN("A reset is requested")
        {
            THEN("The generator should be set to uniform distribution - equal "
                 "probability")
            {
                REQUIRE_CALL(generator, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }
}
