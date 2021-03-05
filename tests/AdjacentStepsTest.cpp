#include "AdjacentSteps.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::AdjacentSteps: default construction")
{
    using namespace aleatoric;

    AdjacentSteps instance(std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
    }

    THEN("A set of numbers should be as expected")
    {
        std::vector<std::vector<int>> possibleResults {{0, 1, 0, 1, 0, 1},
                                                       {1, 0, 1, 0, 1, 0}};

        std::vector<int> set(6);
        for(auto &&i : set) {
            i = instance.getIntegerNumber();
        }

        REQUIRE_THAT(set,
                     Catch::Equals(possibleResults[0]) ||
                         Catch::Equals(possibleResults[1]));
    }
}

SCENARIO("Numbers::AdjacentSteps: constructed with params")
{
    using namespace aleatoric;

    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        Range range(1, 3);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(range.size, 1.0));
                AdjacentSteps(std::move(generator), range);
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

        Range range(1, 3);

        int generatedNumber = 1;
        AdjacentSteps instance(std::move(generator), range);

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
    }
}

SCENARIO("Numbers::AdjacentSteps: params")
{
    using namespace aleatoric;

    Range range(1, 3);
    auto generator = std::make_unique<DiscreteGenerator>();
    auto generatorPointer = generator.get();
    AdjacentSteps instance(std::move(generator), range);

    WHEN("Get params")
    {
        THEN("should match state of the object")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == range.start);
            REQUIRE(returnedRange.end == range.end);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocol::Type::adjacentSteps);
        }
    }

    WHEN("Set params")
    {
        Range newRange(20, 40);
        NumberProtocolConfig newParams(
            newRange,
            NumberProtocolParams(AdjacentStepsParams()));
        instance.setParams(newParams);

        THEN("object state is updated")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }
    }

    WHEN("Set params: Range")
    {
        GIVEN("No numbers have been returned yet")
        {
            Range newRange(4, 7);
            NumberProtocolConfig newParams(
                newRange,
                NumberProtocolParams(AdjacentStepsParams()));
            instance.setParams(newParams);

            THEN("Generator distribution has equal probability for whole range")
            {
                std::vector<double> expectedDistribution {1, 1, 1, 1};
                REQUIRE(generatorPointer->getDistributionVector() ==
                        expectedDistribution);
            }
        }

        GIVEN("There is a last number returned")
        {
            auto lastNumber = instance.getIntegerNumber();

            WHEN("It is outside the new range")
            {
                Range newRange(lastNumber + 1, lastNumber + 5);
                NumberProtocolConfig newParams(
                    newRange,
                    NumberProtocolParams(AdjacentStepsParams()));
                instance.setParams(newParams);

                THEN("Generator dist has equal prob for whole range")
                {
                    std::vector<double> expectedDistribution {1, 1, 1, 1, 1};
                    REQUIRE(generatorPointer->getDistributionVector() ==
                            expectedDistribution);
                }
            }

            WHEN("It is mid-range within new range")
            {
                Range newRange(lastNumber - 2, lastNumber + 2);
                NumberProtocolConfig newParams(
                    newRange,
                    NumberProtocolParams(AdjacentStepsParams()));
                instance.setParams(newParams);

                THEN("Only numbers either side of it are selectable")
                {
                    std::vector<double> expectedDist {0, 1, 0, 1, 0};
                    REQUIRE(generatorPointer->getDistributionVector() ==
                            expectedDist);
                }
            }

            WHEN("It is start of the new range")
            {
                Range newRange(lastNumber, lastNumber + 4);
                NumberProtocolConfig newParams(
                    newRange,
                    NumberProtocolParams(AdjacentStepsParams()));
                instance.setParams(newParams);

                THEN("Only the number above is selectable")
                {
                    std::vector<double> expectedDist {0, 1, 0, 0, 0};
                    REQUIRE(generatorPointer->getDistributionVector() ==
                            expectedDist);
                }
            }

            WHEN("It is end of the new range")
            {
                Range newRange(lastNumber - 4, lastNumber);
                NumberProtocolConfig newParams(
                    newRange,
                    NumberProtocolParams(AdjacentStepsParams()));
                instance.setParams(newParams);

                THEN("Only the number below is selectable")
                {
                    std::vector<double> expectedDist {0, 0, 0, 1, 0};
                    REQUIRE(generatorPointer->getDistributionVector() ==
                            expectedDist);
                }
            }
        }
    }
}
