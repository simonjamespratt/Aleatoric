#include "GranularWalk.hpp"

#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::GranularWalk")
{
    int internalRangeMin = 0;
    int internalRangeMax = 65000;

    GIVEN("The class is instantiated with an invalid deviation factor")
    {
        WHEN("The value provided is less than 0.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                double invalidDeviation = -0.01;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Granular::GranularWalk(
                        std::make_unique<actlib::Numbers::UniformGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(10, 20),
                        invalidDeviation),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Granular::GranularWalk(
                        std::make_unique<actlib::Numbers::UniformGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(10, 20),
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
                    actlib::Numbers::Granular::GranularWalk(
                        std::make_unique<actlib::Numbers::UniformGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(10, 20),
                        invalidDeviation),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Granular::GranularWalk(
                        std::make_unique<actlib::Numbers::UniformGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(10, 20),
                        invalidDeviation),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        double deviationFactor = 0.25;

        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<actlib::Numbers::Range>(10, 20);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("It sets the generator range to that of the internal range")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistribution(internalRangeMin, internalRangeMax));

                actlib::Numbers::Granular::GranularWalk(std::move(generator),
                                                        std::move(range),
                                                        deviationFactor);
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        auto range = std::make_unique<actlib::Numbers::Range>(10, 20);
        auto rangePointer = range.get();

        double deviationFactor = 0.25;
        auto expectedMaxStep = deviationFactor * internalRangeMax;
        int internalRangeMidValue = 32500;

        actlib::Numbers::Granular::GranularWalk instance(std::move(generator),
                                                         std::move(range),
                                                         deviationFactor);

        WHEN("A number is requested")
        {
            THEN("A random number from within the internal range is generated")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(internalRangeMidValue);
                instance.getNumber();
            }

            THEN("That generated number is normalised and returned, scaled to "
                 "the range provided by the calling client")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(internalRangeMidValue);
                auto returnedNumber = instance.getNumber();
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

                    instance.getNumber();
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

                    instance.getNumber();
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

                    instance.getNumber();
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

                    auto roundingRange =
                        std::make_unique<actlib::Numbers::Range>(10, 20);
                    auto roundingRangePointer = roundingRange.get();

                    actlib::Numbers::Granular::GranularWalk
                        instanceNeedingStepRounding(
                            std::move(roundingGenerator),
                            std::move(roundingRange),
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

                    instanceNeedingStepRounding.getNumber();
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

    GIVEN("The class is instantiated with an invalid initial number selection")
    {
        WHEN("The value provided is greater than the range end provided by the "
             "calling client")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelection = 21;

                {
                    auto generator = std::make_unique<UniformGeneratorMock>();
                    auto generatorPointer = generator.get();
                    ALLOW_CALL(*generatorPointer,
                               setDistribution(ANY(int), ANY(int)));

                    REQUIRE_THROWS_AS(
                        actlib::Numbers::Granular::GranularWalk(
                            std::move(generator),
                            std::make_unique<actlib::Numbers::Range>(10, 20),
                            0.25,
                            initialSelection),
                        std::invalid_argument);
                }

                {
                    auto generator = std::make_unique<UniformGeneratorMock>();
                    auto generatorPointer = generator.get();
                    ALLOW_CALL(*generatorPointer,
                               setDistribution(ANY(int), ANY(int)));

                    REQUIRE_THROWS_WITH(
                        actlib::Numbers::Granular::GranularWalk(
                            std::move(generator),
                            std::make_unique<actlib::Numbers::Range>(10, 20),
                            0.25,
                            initialSelection),
                        "The value passed as argument for "
                        "initialSelection must be "
                        "within the range of 10 to 20");
                }
            }
        }

        WHEN("The value provided is less than the range start provided by the "
             "calling client")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelection = 9;

                {
                    auto generator = std::make_unique<UniformGeneratorMock>();
                    auto generatorPointer = generator.get();
                    ALLOW_CALL(*generatorPointer,
                               setDistribution(ANY(int), ANY(int)));

                    REQUIRE_THROWS_AS(
                        actlib::Numbers::Granular::GranularWalk(
                            std::move(generator),
                            std::make_unique<actlib::Numbers::Range>(10, 20),
                            0.25,
                            initialSelection),
                        std::invalid_argument);
                }

                {
                    auto generator = std::make_unique<UniformGeneratorMock>();
                    auto generatorPointer = generator.get();
                    ALLOW_CALL(*generatorPointer,
                               setDistribution(ANY(int), ANY(int)));

                    REQUIRE_THROWS_WITH(
                        actlib::Numbers::Granular::GranularWalk(
                            std::move(generator),
                            std::make_unique<actlib::Numbers::Range>(10, 20),
                            0.25,
                            initialSelection),
                        "The value passed as argument for "
                        "initialSelection must be "
                        "within the range of 10 to 20");
                }
            }
        }
    }

    GIVEN("The class is instantiated with a valid initial number selection")
    {
        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));

        auto range = std::make_unique<actlib::Numbers::Range>(10, 20);
        auto rangePointer = range.get();

        double deviationFactor = 0.25;
        int initialSelection = 15; // halfway between range start and end
        auto expectedMaxStep = deviationFactor * internalRangeMax;
        int internalRangeHalfwayValue = 32500;

        actlib::Numbers::Granular::GranularWalk instance(std::move(generator),
                                                         std::move(range),
                                                         deviationFactor,
                                                         initialSelection);

        WHEN("The first number is requested")
        {
            THEN("It should not call the generator for a number, but return "
                 "the initial selection")
            {
                FORBID_CALL(*generatorPointer, getNumber());
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == initialSelection);
            }

            AND_WHEN("The initial selection does not force rounding when "
                     "scaled to the internal range")
            {
                THEN("It sets the generator distribution with the maxStep sub "
                     "range applied, having scaled the initialSelection "
                     "provided, to the internal range")
                {
                    // NB: with the set up here, no rounding adjustment will
                    // occur

                    // Halfway initial selection within external range should
                    // map to halfway within internal range. Therefore, the
                    // maxStep should match to either side of that number. So:

                    // halfway on internal range: 65000 / 2 = 32500
                    // maxStep = 0.25 * 65000 = 16250
                    // therefore, setDistribution should be called with:

                    // (32500 - 16250) and (32500 + 16250)

                    REQUIRE_CALL(
                        *generatorPointer,
                        setDistribution(
                            (internalRangeHalfwayValue - expectedMaxStep),
                            (internalRangeHalfwayValue + expectedMaxStep)));
                    instance.getNumber();
                }
            }

            AND_WHEN("The initial selection does force rounding when scaled to "
                     "the internal range")
            {
                THEN("It sets the generator distribution with the maxStep sub "
                     "range applied, but to a rounded version of the scaled "
                     "initial selection")
                {
                    auto roundingGenerator =
                        std::make_unique<UniformGeneratorMock>();
                    auto roundingGeneratorPointer = roundingGenerator.get();

                    ALLOW_CALL(*roundingGeneratorPointer,
                               setDistribution(ANY(int), ANY(int)));

                    auto rangeRoundingRequired =
                        std::make_unique<actlib::Numbers::Range>(0, 9);
                    auto rangeRoundingRequiredPointer =
                        rangeRoundingRequired.get();

                    int initSelection = 3;

                    actlib::Numbers::Granular::GranularWalk
                        instanceWithRoundingRequired(
                            std::move(roundingGenerator),
                            std::move(rangeRoundingRequired),
                            deviationFactor,
                            initSelection);

                    // NB: Rounding here will be required:

                    // A normalised initial selection here will result in a
                    // number with a fractional part which will be rounded:

                    // normalised: 3 / 9 = 0.333...
                    // scaled to internal range: 0.333... * 65000 = 21666.666...
                    // which will be rounded to: 21667
                    // and the maxStep sub-range will be set around that number:
                    // setDistribution called with:
                    // (21667 - maxStep) and (21667 + maxStep)

                    int expectedScaledInitialSelection = 21667;

                    REQUIRE_CALL(
                        *roundingGeneratorPointer,
                        setDistribution(
                            (expectedScaledInitialSelection - expectedMaxStep),
                            (expectedScaledInitialSelection +
                             expectedMaxStep)));

                    instanceWithRoundingRequired.getNumber();
                }
            }
        }

        WHEN("A subsequent number is requested")
        {
            instance.getNumber(); // first call

            THEN("It does call the generator for a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).TIMES(1).RETURN(1);
                instance.getNumber();
            }

            AND_THEN("It normalises the generated number and returns it scaled "
                     "to the range provided by the calling client")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .TIMES(1)
                    .RETURN(internalRangeHalfwayValue);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == 15.0);
            }

            AND_THEN("It sets the generator distribution in readiness for the "
                     "next call to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).TIMES(1).RETURN(1);
                REQUIRE_CALL(*generatorPointer,
                             setDistribution(ANY(int), ANY(int)));
                instance.getNumber();
            }
        }

        WHEN("A reset is performed following a previous call to get a number")
        {
            instance.getNumber(); // unsets the initial state

            AND_WHEN("The next number is requested")
            {
                THEN("The initial selection is returned, due to the initial "
                     "state having been reinstated during the reset")
                {
                    instance.reset();
                    FORBID_CALL(*generatorPointer, getNumber());
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber == initialSelection);
                }
            }
        }
    }
}
