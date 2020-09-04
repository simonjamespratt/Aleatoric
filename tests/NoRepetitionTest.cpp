#include "NoRepetition.hpp"

#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::NoRepetition")
{
    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        aleatoric::Range range(1, 3);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(range.size, 1.0));
                aleatoric::NoRepetition(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int generatedNumber = 1;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, updateDistributionVector(1.0));
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(int), 0.0));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        aleatoric::Range range(1, 3);

        aleatoric::NoRepetition instance(std::move(generator), range);

        WHEN("A number is requested")
        {
            THEN("It should request a number from the generator")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                instance.getIntegerNumber();
            }

            THEN("It should return the generated number with the range offset "
                 "applied")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            THEN("It should ensure that the generated number cannot be "
                 "selected at the next time of asking whilst ensuring all "
                 "others have "
                 "equal probability of being selected")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getIntegerNumber();
            }
        }

        WHEN("The ranged is changed")
        {
            ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));
            aleatoric::Range newRange(2, 10);

            THEN("The returned range should match the new range")
            {
                instance.setRange(newRange);
                auto returnedRange = instance.getRange();
                REQUIRE(returnedRange.start == 2);
                REQUIRE(returnedRange.end == 10);
            }

            THEN("The generator should be updated")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(newRange.size, 1.0));
                instance.setRange(newRange);
            }

            AND_WHEN("The last returned number is within the new range")
            {
                auto lastNumberGenerated = range.size - 1;

                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(lastNumberGenerated);

                auto lastNumberReturned = instance.getIntegerNumber();

                THEN("That number should be disallowed")
                {
                    REQUIRE_CALL(
                        *generatorPointer,
                        updateDistributionVector(lastNumberGenerated, 0.0));

                    instance.setRange(aleatoric::Range(lastNumberReturned - 1,
                                                       lastNumberReturned + 1));
                }
            }

            AND_WHEN("The last returned number is outside the new range")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(range.size - 1);

                auto lastNumberReturned = instance.getIntegerNumber();

                THEN("No additional treatment is required")
                {
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(ANY(int), ANY(double)));

                    instance.setRange(aleatoric::Range(lastNumberReturned + 1,
                                                       lastNumberReturned + 3));

                    instance.setRange(aleatoric::Range(lastNumberReturned - 3,
                                                       lastNumberReturned - 1));
                }
            }
        }

        WHEN("A reset is requested")
        {
            THEN("The generator should be set to uniform distribution - equal "
                 "probability")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }
}
