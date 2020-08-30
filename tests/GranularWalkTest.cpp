#include "GranularWalk.hpp"

#include "NumberProtocolParameters.hpp"
#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::GranularWalk: default constructor")
{
    using namespace aleatoric;

    GranularWalk instance(std::make_unique<UniformGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto deviationFactor =
            params.protocols.getGranularWalk().getDeviationFactor();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(deviationFactor == 1.0);
    }

    THEN("All numbers returned should be within default range")
    {
        for(int i = 0; i < 1000; i++) {
            auto number = instance.getDecimalNumber();
            REQUIRE(number >= 0);
            REQUIRE(number <= 1);
        }
    }
}

SCENARIO("Numbers::GranularWalk")
{
    using namespace aleatoric;

    int internalRangeMin = 0;
    int internalRangeMax = 65000;

    GIVEN("Construction: with an invalid deviation factor")
    {
        WHEN("The value provided is less than 0.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                double invalidDeviation = -0.01;

                REQUIRE_THROWS_AS(
                    GranularWalk(std::make_unique<UniformGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    GranularWalk(std::make_unique<UniformGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }

        WHEN("The value provided is more than 1.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                double invalidDeviation = 1.01;
                REQUIRE_THROWS_AS(
                    GranularWalk(std::make_unique<UniformGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    GranularWalk(std::make_unique<UniformGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }
    }

    GIVEN("Construction")
    {
        double deviationFactor = 0.25;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        auto range = Range(10, 20);

        WHEN("The object is constructed")
        {
            THEN("It sets the generator range to that of the internal range")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(internalRangeMin, internalRangeMax));

                GranularWalk(std::move(generator), range, deviationFactor);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        auto range = Range(10, 20);

        double deviationFactor = 0.25;
        auto expectedMaxStep = deviationFactor * internalRangeMax;
        int internalRangeMidValue = 32500;

        GranularWalk instance(std::move(generator), range, deviationFactor);

        WHEN("A number is requested")
        {
            THEN("A random number from within the internal range is generated")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(internalRangeMidValue);
                instance.getDecimalNumber();
            }

            THEN("That generated number is normalised and returned, scaled to "
                 "the range provided by the calling client")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(internalRangeMidValue);
                auto returnedNumber = instance.getDecimalNumber();
                REQUIRE(returnedNumber == 15.0); // ((20 - 10) / 2) + 10
            }

            AND_WHEN("That generated number is mid range")
            {
                // mid-range here meaning that the new distribution range
                // won't exceed the main range
                THEN("It sets the generator to the maxStep sub range in "
                     "readiness for the next call to get a number")
                {
                    // The sub range should be the maxStep in both directions
                    // with the last selected number in the middle. Range is
                    // inclusive. i.e:

                    // rangeStart = lastSelected - maxStep,
                    // rangeEnd = lastSelected + maxStep

                    // The maxStep is calculated from the deviationFactor
                    // provided upon instantiation. The calculation is
                    // essentially:

                    // dF * internalRange - rounded if necessary
                    // so here, it should be 0.25 * 65000 = 16250

                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(internalRangeMidValue);
                    REQUIRE_CALL(
                        *generatorPointer,
                        setDistribution(
                            (internalRangeMidValue - expectedMaxStep),
                            (internalRangeMidValue + expectedMaxStep)));

                    instance.getDecimalNumber();
                }
            }

            AND_WHEN("That generated number is at the start of the range")
            {
                // start-of-range here meaning that the new distribution range
                // set will exceed the main range start
                THEN("It sets the generator to the maxStep sub range in "
                     "readiness for the next call to get a number")
                {
                    // The logic does not account for wrapping and maxStep range
                    // is curtailed if it hits either end of the main range
                    int startOfRangeNumber = internalRangeMin + 1;
                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(startOfRangeNumber);
                    REQUIRE_CALL(*generatorPointer,
                                 setDistribution(
                                     internalRangeMin,
                                     (startOfRangeNumber + expectedMaxStep)));

                    instance.getDecimalNumber();
                }
            }

            AND_WHEN("That generated number is end of range")
            {
                // end-of-range here meaning that the new distribution range
                // set will exceed the main range end
                THEN("It sets the generator to the maxStep sub range in "
                     "readiness for the next call to get a number")
                {
                    // The logic does not account for wrapping and maxStep range
                    // is curtailed if it hits either end of the main range
                    int endOfRangeNumber = internalRangeMax - 1;
                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(endOfRangeNumber);
                    REQUIRE_CALL(
                        *generatorPointer,
                        setDistribution((endOfRangeNumber - expectedMaxStep),
                                        internalRangeMax));

                    instance.getDecimalNumber();
                }
            }

            AND_WHEN(
                "The maxStep has been calculated where rounding was required")
            {
                THEN("The generator is set with the rounded maxStep being used "
                     "to create the sub range")
                {
                    double devFactorThatCausesRounding = 0.12345;

                    auto roundingGenerator =
                        std::make_unique<UniformGeneratorMock>();
                    auto roundingGeneratorPointer = roundingGenerator.get();

                    ALLOW_CALL(*roundingGeneratorPointer,
                               setDistribution(ANY(int), ANY(int)));

                    auto roundingRange = Range(10, 20);

                    GranularWalk instanceNeedingStepRounding(
                        std::move(roundingGenerator),
                        roundingRange,
                        devFactorThatCausesRounding);

                    // With this set up, the scaling of the deviationFactor will
                    // result in a maxStep number with a fractional part:

                    // 0.12345 * 65000 = 8024.25
                    // which will be rounded down to 8024
                    // which will be the maxStep used for creating the sub
                    // range, so, setDistribution should be called with:
                    // (generatedNumber - 8024) and (generatedNumber + 8024)

                    int expectedMaxStepRounded = 8024;
                    REQUIRE_CALL(*roundingGeneratorPointer, getNumber())
                        .RETURN(internalRangeMidValue);
                    REQUIRE_CALL(
                        *roundingGeneratorPointer,
                        setDistribution(
                            (internalRangeMidValue - expectedMaxStepRounded),
                            (internalRangeMidValue + expectedMaxStepRounded)));

                    instanceNeedingStepRounding.getDecimalNumber();
                }
            }
        }

        WHEN("A reset is performed")
        {
            THEN("The generator distribution is set to the full internal range")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(internalRangeMin, internalRangeMax));
                instance.reset();
            }
        }
    }
}

SCENARIO("Numbers::GranularWalk: params")
{
    using namespace aleatoric;

    GranularWalk instance(std::make_unique<UniformGenerator>(),
                          Range(10, 20),
                          0.5);

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("range should match the state of the object")
        {
            REQUIRE(returnedRange.start == 10);
            REQUIRE(returnedRange.end == 20);
        }

        THEN("deviation factor should match state of object")
        {
            REQUIRE(params.protocols.getGranularWalk().getDeviationFactor() ==
                    0.5);
        }

        THEN("active protocol should be granular walk")
        {
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocolParameters::Protocols::ActiveProtocol::
                        granularWalk);
        }
    }

    WHEN("Set params")
    {
        Range newRange(20, 40);
        double newDeviationFactor = 0.1;
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::GranularWalk(newDeviationFactor)));

        instance.setParams(newParams);
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("range is updated")
        {
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }

        THEN("deviation factor is updated")
        {
            REQUIRE(params.protocols.getGranularWalk().getDeviationFactor() ==
                    newDeviationFactor);
        }

        AND_WHEN("A set of numbers is gathered")
        {
            std::vector<double> set(1000);
            for(auto &&i : set) {
                i = instance.getDecimalNumber();
            }

            THEN("All numbers should be in the new range")
            {
                for(auto &&i : set) {
                    REQUIRE_FALSE(i < newRange.start);
                    REQUIRE_FALSE(i > newRange.end);
                }
            }

            THEN("All numbers should no more than the new deviation factor "
                 "from the last number")
            {
                double newMaxStep =
                    (newRange.end - newRange.start) * newDeviationFactor; // 2

                std::vector<double> differences(set.size());
                std::adjacent_difference(set.begin(),
                                         set.end(),
                                         differences.begin());

                //  remove the first element as it is the value of the first
                //  element in the set (and not a difference between two
                //  numbers in the set)
                differences.erase(differences.begin());

                for(auto &&i : differences) {
                    REQUIRE(i <= newMaxStep);
                }
            }
        }
    }

    WHEN("Set params: Last returned number is within new range")
    {
        auto lastNumber = instance.getDecimalNumber();

        double newDeviationFactor = 0.1;
        Range newRange(lastNumber - 10, lastNumber + 10);

        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::GranularWalk(newDeviationFactor)));

        instance.setParams(newParams);

        auto nextNumber = instance.getDecimalNumber();

        THEN("The next number should be no more than the new deviation factor "
             "from the last number")
        {
            double newMaxStep =
                (newRange.end - newRange.start) * newDeviationFactor; // 2

            auto difference = std::abs(lastNumber - nextNumber);
            REQUIRE(difference <= newMaxStep);
        }

        THEN("The next number should be within the new range")
        {
            REQUIRE_FALSE(nextNumber < newRange.start);
            REQUIRE_FALSE(nextNumber > newRange.end);
        }
    }

    WHEN("Set params: invalid deviation factor")
    {
        double devFactorTooHigh = 1.1;
        double devFactorTooLow = -0.1;

        Range newRange(20, 40);

        THEN("Throw exception if too high")
        {
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::GranularWalk(devFactorTooHigh)));

            REQUIRE_THROWS_AS(instance.setParams(newParams),
                              std::invalid_argument);
        }

        THEN("Throw exception if too low")
        {
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::GranularWalk(devFactorTooLow));
            REQUIRE_THROWS_AS(instance.setParams(newParams),
                              std::invalid_argument);
        }
    }
}
