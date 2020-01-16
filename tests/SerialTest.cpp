#include "Serial.hpp"

#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <array>
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Serial")
{
    GIVEN("The class is instantiated correctly")
    {
        actlib::Numbers::Range range(1, 3);

        DiscreteGeneratorMock generator;
        actlib::Numbers::Steps::Serial instance(generator, range);

        int generatedNumber = 1;

        // TODO: can these be set to ANY?
        ALLOW_CALL(generator, getNumber()).RETURN(generatedNumber);
        ALLOW_CALL(generator, updateDistributionVector(generatedNumber, 0.0));

        // this is for reset()
        ALLOW_CALL(generator, updateDistributionVector(1.0));

        // ensures that seriesIsComplete returns true
        ALLOW_CALL(generator, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                instance.getNumber();
            }

            THEN("It updates the generator to disallow the selected number "
                 "from being selected in future calls")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                REQUIRE_CALL(generator,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getNumber();
            }

            THEN("It returns the selected number, offset by the range.offset")
            {
                REQUIRE_CALL(generator, getNumber()).RETURN(generatedNumber);
                auto number = instance.getNumber();
                REQUIRE(number == generatedNumber + range.offset);
            }

            THEN("It gets the state of the generator distribution for "
                 "determining if the series is complete")
            {
                // NB: the return value for this test can be anything
                REQUIRE_CALL(generator, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                instance.getNumber();
            }

            AND_WHEN("The series is complete")
            {
                THEN("It resets the generator distribution")
                {
                    REQUIRE_CALL(generator, getDistributionVector())
                        .RETURN(std::vector<double> {0.0});
                    REQUIRE_CALL(generator, updateDistributionVector(1.0));
                    instance.getNumber();
                }
            }

            AND_WHEN("The series is not complete")
            {
                THEN("It does not reset the generator distribution")
                {
                    REQUIRE_CALL(generator, getDistributionVector())
                        .RETURN(std::vector<double> {1.0});
                    FORBID_CALL(generator, updateDistributionVector(1.0));
                    instance.getNumber();
                }
            }
        }

        WHEN("A reset is enforced")
        {
            THEN("It resets the generator distribution")
            {
                REQUIRE_CALL(generator, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }
}
