#include "Precision.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Precision")
{
    GIVEN("Construction: with an invalid distribution")
    {
        WHEN("The sum of the values in the distribution is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                REQUIRE_THROWS_AS(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        std::vector<double> {0.99}),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        std::vector<double> {0.99}),
                    "The sum of the values provided as the vector for the "
                    "distribution must equal 1.0");
            }
        }

        WHEN("The sum of the values in the distribution is more than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                REQUIRE_THROWS_AS(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        std::vector<double> {1.01}),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        std::vector<double> {1.01}),
                    "The sum of the values provided as the vector for the "
                    "distribution must equal 1.0");
            }
        }

        WHEN("The size of the distribution vector does not match the size of "
             "the range")
        {
            REQUIRE_THROWS_AS(
                aleatoric::Precision(
                    std::make_unique<aleatoric::DiscreteGenerator>(),
                    aleatoric::Range(1, 4),
                    std::vector<double> {1.0}),
                std::invalid_argument);

            REQUIRE_THROWS_WITH(
                aleatoric::Precision(
                    std::make_unique<aleatoric::DiscreteGenerator>(),
                    aleatoric::Range(1, 4),
                    std::vector<double> {1.0}),
                "The vector size for the distribution must match the size of "
                "the provided range");
        }
    }

    GIVEN("Construction: no initialSelection")
    {
        WHEN("The object is constructed")
        {
            auto generator = std::make_unique<DiscreteGeneratorMock>();
            auto generatorPointer = generator.get();

            aleatoric::Range range(1, 4);

            std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};

            THEN("The generator distribution should have been set with the "
                 "received distribution")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(distribution));

                aleatoric::Precision(std::move(generator), range, distribution);
            }
        }
    }

    GIVEN("The object is constructed: no initialSelection")
    {
        std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistributionVector(distribution));

        aleatoric::Range range(1, 4);

        aleatoric::Precision instance(std::move(generator),
                                      range,
                                      distribution);

        WHEN("A number is requested")
        {
            THEN("It returns a generated number with the range offset added")
            {
                int generatedNumber = 1;

                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }
        }

        WHEN("The range is changed")
        {
            ALLOW_CALL(*generatorPointer, getDistributionVector())
                .RETURN(distribution);
            ALLOW_CALL(*generatorPointer,
                       setDistributionVector(ANY(std::vector<double>)));

            THEN("The returned range should match the new range")
            {
                instance.setRange(aleatoric::Range(2, 10));
                auto returnedRange = instance.getRange();
                REQUIRE(returnedRange.start == 2);
                REQUIRE(returnedRange.end == 10);
            }

            // TODO: DYNAMIC-PARAMS: this is an interim measure and should be
            // updated when the solution for altering all params at the same
            // time is implemented.
            AND_WHEN("The new range size is larger than the original range")
            {
                aleatoric::Range largerRange(0, 5);

                THEN("The difference should be added to the distribution where "
                     "each additional value is set to 0.0")
                {
                    REQUIRE_CALL(
                        *generatorPointer,
                        setDistributionVector(std::vector<double> {0.25,
                                                                   0.25,
                                                                   0.25,
                                                                   0.25,
                                                                   0,
                                                                   0}));
                    instance.setRange(largerRange);
                }
            }

            // TODO: DYNAMIC-PARAMS: this is an interim measure and should be
            // updated when the solution for altering all params at the same
            // time is implemented.
            AND_WHEN("The new range is smaller than the original range")
            {
                aleatoric::Range smallerRange(2, 3);

                THEN("The difference should be removed from the end of the "
                     "distribution")
                {
                    REQUIRE_CALL(*generatorPointer,
                                 setDistributionVector(
                                     std::vector<double> {0.25, 0.25}));
                    instance.setRange(smallerRange);
                }
            }

            AND_WHEN("The range size is the same as the original range")
            {
                aleatoric::Range sameSizeRange(2, 5);

                THEN("No further treatment is required")
                {
                    FORBID_CALL(
                        *generatorPointer,
                        setDistributionVector(ANY(std::vector<double>)));
                    instance.setRange(sameSizeRange);
                }
            }
        }
    }

    GIVEN("Construction: with an invalid initialSelection")
    {
        std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};

        WHEN("The value provided is greater than the range end")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelectionOutOfRange = 5;

                REQUIRE_THROWS_AS(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        distribution,
                        initialSelectionOutOfRange),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        distribution,
                        initialSelectionOutOfRange),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 4");
            }
        }

        WHEN("The value provided is less than the range start")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelectionOutOfRange = 0;

                REQUIRE_THROWS_AS(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        distribution,
                        initialSelectionOutOfRange),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Precision(
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(1, 4),
                        distribution,
                        initialSelectionOutOfRange),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 4");
            }
        }
    }

    GIVEN("The object is constructed: with initialSelection")
    {
        std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};

        int initialSelection = 2;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistributionVector(distribution));

        aleatoric::Range range(1, 4);

        aleatoric::Precision instance(std::move(generator),
                                      range,
                                      distribution,
                                      initialSelection);

        WHEN("The first number is requested")
        {
            THEN("It should return the value provided as the initial selection")
            {
                FORBID_CALL(*generatorPointer, getNumber());

                int returnedNumber = instance.getIntegerNumber();

                REQUIRE(returnedNumber == initialSelection);
            }
        }

        WHEN("A subsequent number is requested")
        {
            THEN("It returns a generated number with the range offset added")
            {
                int generatedNumber = 1;

                instance.getIntegerNumber(); // first call

                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);

                auto returnedNumner = instance.getIntegerNumber();

                REQUIRE(returnedNumner == generatedNumber + range.offset);
            }
        }

        WHEN("A reset is requested")
        {
            AND_WHEN("A number is requested")
            {
                THEN("The number returned should be the value provided as the "
                     "initial selection")
                {
                    instance.reset();

                    FORBID_CALL(*generatorPointer, getNumber());

                    int returnedNumber = instance.getIntegerNumber();

                    REQUIRE(returnedNumber == initialSelection);
                }
            }

            AND_WHEN("A subsequent number is requested")
            {
                THEN(
                    "It returns a generated number with the range offset added")
                {
                    instance.reset();
                    instance.getIntegerNumber(); // first call
                    int generatedNumber = 1;

                    REQUIRE_CALL(*generatorPointer, getNumber())
                        .RETURN(generatedNumber);

                    auto returnedNumner =
                        instance.getIntegerNumber(); // second call

                    REQUIRE(returnedNumner == generatedNumber + range.offset);
                }
            }
        }
    }
}
