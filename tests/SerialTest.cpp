#include "Serial.hpp"

#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <array>
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Serial")
{
    GIVEN("The class is instantiated")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<aleatoric::Range>(1, 3);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                aleatoric::Serial(std::move(generator), std::move(range));
            }
        }
    }

    GIVEN("The class is instantiated")
    {
        int generatedNumber = 1;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(generatedNumber);
        ALLOW_CALL(*generatorPointer,
                   updateDistributionVector(generatedNumber, 0.0));

        // this is for reset()
        ALLOW_CALL(*generatorPointer, updateDistributionVector(1.0));

        // ensures that seriesIsComplete returns true
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        auto range = std::make_unique<aleatoric::Range>(1, 3);
        auto rangePointer = range.get();

        aleatoric::Serial instance(std::move(generator), std::move(range));

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                instance.getIntegerNumber();
            }

            THEN("It updates the generator to disallow the selected number "
                 "from being selected in future calls")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getIntegerNumber();
            }

            THEN("It returns the selected number, offset by the range.offset")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto number = instance.getIntegerNumber();
                REQUIRE(number == generatedNumber + rangePointer->offset);
            }

            THEN("It gets the state of the generator distribution for "
                 "determining if the series is complete")
            {
                // NB: the return value for this test can be anything
                REQUIRE_CALL(*generatorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                instance.getIntegerNumber();
            }

            AND_WHEN("The series is complete")
            {
                THEN("It resets the generator distribution")
                {
                    REQUIRE_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {0.0});
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(1.0));
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("The series is not complete")
            {
                THEN("It does not reset the generator distribution")
                {
                    REQUIRE_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {1.0});
                    FORBID_CALL(*generatorPointer,
                                updateDistributionVector(1.0));
                    instance.getIntegerNumber();
                }
            }
        }

        WHEN("A reset is requested")
        {
            THEN("It resets the generator distribution")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }
}
