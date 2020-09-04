#include "Periodic.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Periodic")
{
    GIVEN("Construction: with an invalid chanceOfRepetition value")
    {
        WHEN("The value provided is greater than 1.0")
        {
            THEN("A standard invalid_argument exeception is thrown")
            {
                double invalidChanceValue = 1.1;

                REQUIRE_THROWS_AS(
                    aleatoric::Periodic(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 3),
                        invalidChanceValue),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Periodic(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 3),
                        invalidChanceValue),
                    "The value passed as argument for chanceOfRepetition must "
                    "be within the range of 0.0 - 1.0");
            }
        }

        WHEN("The value provided is less than 0.0")
        {
            THEN("A standard invalid_argument exeception is thrown")
            {
                double invalidChanceValue = -0.1;

                REQUIRE_THROWS_AS(
                    aleatoric::Periodic(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 3),
                        invalidChanceValue),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Periodic(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 3),
                        invalidChanceValue),
                    "The value passed as argument for chanceOfRepetition must "
                    "be within the range of 0.0 - 1.0");
            }
        }
    }

    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(initialGeneratorDistributionState);

        aleatoric::Range range(1, 3);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(range.size, 1.0));
                aleatoric::Periodic(std::move(generator), range, 0.5);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};

        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(initialGeneratorDistributionState);
        ALLOW_CALL(*generatorPointer,
                   setDistributionVector(ANY(std::vector<double>)));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        aleatoric::Range range(1, 3);

        double chanceOfRepetition = 0.5;

        aleatoric::Periodic instance(std::move(generator),
                                     range,
                                     chanceOfRepetition);

        int generatedNumber = 1; // mid range selection

        WHEN("A number is requested")
        {
            THEN("It returns a generated number with the range offset added")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            AND_THEN("The generator distibution should be set with a bias in "
                     "favour of the last generated number according to the "
                     "periodicity received during construction")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer, getDistributionVector())
                    .TIMES(1) // doesn't seem to register that it is called in
                              // the constructor as well
                    .RETURN(initialGeneratorDistributionState);

                // The total of all values in the vector must equal 1.0. The
                // value at the index of the last selected number must have the
                // value of the periodicity (chanceOfRepetition). The remainder
                // of 1.0 - periodicity is shared equally amongst the remaining
                // vector indices.
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistributionVector(
                        std::vector<double> {0.25, chanceOfRepetition, 0.25}));
                instance.getIntegerNumber();
            }
        }

        WHEN("A reset is performed")
        {
            THEN("The generator distribution is set to uniform - equal "
                 "probability of selection for each number in the range upon "
                 "next call to getNumber()")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }
        }

        WHEN("The range is changed")
        {
            REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(range.size - 1);

            auto lastReturnedNumber = instance.getIntegerNumber();

            aleatoric::Range newRangeOutsideAndAbove(lastReturnedNumber + 1,
                                                     lastReturnedNumber + 3);

            aleatoric::Range newRangeOutsideAndBelow(lastReturnedNumber - 5,
                                                     lastReturnedNumber - 1);

            aleatoric::Range newRangeAround(lastReturnedNumber - 2,
                                            lastReturnedNumber + 2);

            THEN("The returned range should match the new range")
            {
                instance.setRange(aleatoric::Range(2, 10));
                auto returnedRange = instance.getRange();
                REQUIRE(returnedRange.start == 2);
                REQUIRE(returnedRange.end == 10);
            }

            THEN("The generator should be reconfigured to equal "
                 "probability of selection")
            {
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistributionVector(newRangeOutsideAndAbove.size, 1.0));
                REQUIRE_CALL(
                    *generatorPointer,
                    setDistributionVector(newRangeOutsideAndBelow.size, 1.0));
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(newRangeAround.size, 1.0));
                instance.setRange(newRangeOutsideAndAbove);
                instance.setRange(newRangeOutsideAndBelow);
                instance.setRange(newRangeAround);
            }

            AND_WHEN("The last returned number is within the new range")
            {
                THEN("The generator should be reconfigured to favour the last "
                     "selected number with correct remainder allocation for "
                     "the other possibilities")
                {
                    // The total of all values in the vector must equal 1.0. The
                    // value at the index of the last selected number must have
                    // the value of the periodicity (chanceOfRepetition). The
                    // remainder of 1.0 - periodicity is shared equally amongst
                    // the remaining vector indices.
                    ALLOW_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {1, 1, 1, 1, 1});
                    REQUIRE_CALL(*generatorPointer,
                                 setDistributionVector(
                                     std::vector<double> {0.125,
                                                          0.125,
                                                          chanceOfRepetition,
                                                          0.125,
                                                          0.125}));
                    instance.setRange(newRangeAround);
                }
            }

            AND_WHEN("The last returned number is outside the new range")
            {
                THEN("No further treatment is required")
                {
                    FORBID_CALL(
                        *generatorPointer,
                        setDistributionVector(ANY(std::vector<double>)));

                    instance.setRange(newRangeOutsideAndAbove);
                    instance.setRange(newRangeOutsideAndBelow);
                }
            }
        }
    }
}
