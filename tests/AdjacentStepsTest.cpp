#include "AdjacentSteps.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::AdjacentSteps")
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
                aleatoric::AdjacentSteps(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(double)));
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(int), 1.0));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        aleatoric::Range range(1, 3);

        int generatedNumber = 1;
        aleatoric::AdjacentSteps instance(std::move(generator), range);

        WHEN("A number is requested")
        {
            THEN("It returns a generated number with the range offset added")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            AND_WHEN("The returned number is mid range")
            {
                auto midRangeGeneratedNumber = 2 - range.offset;
                auto stepUp = midRangeGeneratedNumber + 1;
                auto stepDown = midRangeGeneratedNumber - 1;

                THEN("A stepwise distribution is set such that a step can be "
                     "taken either up or down")
                {
                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(midRangeGeneratedNumber);
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(0.0));
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(stepUp, 1.0));
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(stepDown, 1.0));
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(midRangeGeneratedNumber, 1.0));
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("The returned number is the bottom (start) of the range")
            {
                auto startOfRangeGeneratedNumber = range.start - range.offset;
                auto stepUp = startOfRangeGeneratedNumber + 1;
                auto stepDown = startOfRangeGeneratedNumber - 1;

                THEN("A stepwise distribution is set such that a step can "
                     "only be taken upwards")
                {
                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(startOfRangeGeneratedNumber);
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(0.0));
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(stepUp, 1.0));
                    FORBID_CALL(*generatorPointer,
                                updateDistributionVector(stepDown, 1.0));
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(startOfRangeGeneratedNumber,
                                                 1.0));
                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("The returned number is the top (end) of the range")
            {
                auto endOfRangeGeneratedNumber = range.end - range.offset;
                auto stepUp = endOfRangeGeneratedNumber + 1;
                auto stepDown = endOfRangeGeneratedNumber - 1;

                THEN("A stepwise distribution is set such that a step can "
                     "only be taken downwards")
                {
                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(endOfRangeGeneratedNumber);
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(0.0));
                    FORBID_CALL(*generatorPointer,
                                updateDistributionVector(stepUp, 1.0));
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(stepDown, 1.0));
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(endOfRangeGeneratedNumber,
                                                 1.0));
                    instance.getIntegerNumber();
                }
            }
        }

        WHEN("A reset is made")
        {
            THEN("The generator distribution is set to uniform (equal "
                 "probability")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }

            AND_WHEN("The next number is requested")
            {
                THEN("It calls the generator to get one and returns it with "
                     "the range offset added")
                {
                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(generatedNumber);
                    instance.reset();
                    auto returnedNumber = instance.getIntegerNumber();
                    REQUIRE(returnedNumber == generatedNumber + range.offset);
                }
            }
        }

        WHEN("The range is changed")
        {
            aleatoric::Range newRange(2, 10);

            THEN("The returned range should match the new range")
            {
                instance.setRange(newRange);
                auto returnedRange = instance.getRange();
                REQUIRE(returnedRange.start == 2);
                REQUIRE(returnedRange.end == 10);
            }

            THEN("The generator should be reconfigured to equal probability")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(newRange.size, 1.0));
                instance.setRange(newRange);
            }

            AND_WHEN(
                "The last returned number is mid-range within the new range")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(range.size - 1);
                auto lastReturnedNumber = instance.getIntegerNumber();
                aleatoric::Range newRangeAround(lastReturnedNumber - 2,
                                                lastReturnedNumber + 2);

                THEN("Only the numbers either side of it are selectable")
                {
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(0.0));
                    REQUIRE_CALL(
                        *generatorPointer,
                        updateDistributionVector(
                            (lastReturnedNumber - newRangeAround.offset) - 1,
                            1.0));
                    REQUIRE_CALL(
                        *generatorPointer,
                        updateDistributionVector(
                            (lastReturnedNumber - newRangeAround.offset) + 1,
                            1.0));
                    instance.setRange(newRangeAround);
                }
            }

            AND_WHEN("The last returned number is the start of the new range")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(range.size - 1);
                auto lastReturnedNumber = instance.getIntegerNumber();
                aleatoric::Range newRangeAtStart(lastReturnedNumber,
                                                 lastReturnedNumber + 2);

                THEN("Only the number above should be selectable")
                {
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(0.0));
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(
                            (lastReturnedNumber - newRangeAtStart.offset) - 1,
                            1.0));
                    REQUIRE_CALL(
                        *generatorPointer,
                        updateDistributionVector(
                            (lastReturnedNumber - newRangeAtStart.offset) + 1,
                            1.0));
                    instance.setRange(newRangeAtStart);
                }
            }

            AND_WHEN("The last returned number is the end of the new range")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(range.size - 1);
                auto lastReturnedNumber = instance.getIntegerNumber();
                aleatoric::Range newRangeAtEnd(lastReturnedNumber - 2,
                                               lastReturnedNumber);

                THEN("Only the number below should be selectable")
                {
                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(0.0));
                    REQUIRE_CALL(
                        *generatorPointer,
                        updateDistributionVector(
                            (lastReturnedNumber - newRangeAtEnd.offset) - 1,
                            1.0));
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(
                            (lastReturnedNumber - newRangeAtEnd.offset) + 1,
                            1.0));
                    instance.setRange(newRangeAtEnd);
                }
            }

            AND_WHEN("The last returned number is not within the new range")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(range.size - 1);
                auto lastReturnedNumber = instance.getIntegerNumber();
                aleatoric::Range newRangeOutsideAndAbove(lastReturnedNumber + 1,
                                                         lastReturnedNumber +
                                                             3);

                aleatoric::Range newRangeOutsideAndBelow(lastReturnedNumber - 5,
                                                         lastReturnedNumber -
                                                             1);
                THEN("No further treatment is required")
                {
                    FORBID_CALL(*generatorPointer,
                                updateDistributionVector(0.0));
                    FORBID_CALL(
                        *generatorPointer,
                        updateDistributionVector(ANY(int), ANY(double)));
                    instance.setRange(newRangeOutsideAndAbove);
                    instance.setRange(newRangeOutsideAndBelow);
                }
            }
        }
    }
}
