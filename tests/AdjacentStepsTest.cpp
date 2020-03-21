#include "AdjacentSteps.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <memory>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::AdjacentSteps")
{
    GIVEN("The class is instantiated without an initial number selection")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                actlib::Numbers::Steps::AdjacentSteps(std::move(generator),
                                                      std::move(range));
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(double)));
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(int), 1.0));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        int generatedNumber = 1;
        actlib::Numbers::Steps::AdjacentSteps instance(std::move(generator),
                                                       std::move(range));

        WHEN("A number is requested")
        {
            THEN("It returns a generated number with the range offset added")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber ==
                        generatedNumber + rangePointer->offset);
            }

            AND_WHEN("The returned number is mid range")
            {
                auto midRangeGeneratedNumber = 2 - rangePointer->offset;
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
                    instance.getNumber();
                }
            }

            AND_WHEN("The returned number is the bottom (start) of the range")
            {
                auto startOfRangeGeneratedNumber =
                    rangePointer->start - rangePointer->offset;
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
                    instance.getNumber();
                }
            }

            AND_WHEN("The returned number is the top (end) of the range")
            {
                auto endOfRangeGeneratedNumber =
                    rangePointer->end - rangePointer->offset;
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
                    instance.getNumber();
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
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber ==
                            generatedNumber + rangePointer->offset);
                }
            }
        }
    }

    GIVEN("The class is instantiated with an invalid initialSelection value")
    {
        WHEN("The value provided is greater than the range end")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelectionOutOfRange = 4;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::AdjacentSteps(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::AdjacentSteps(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }

        WHEN("The value provided is less than the range start")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelectionOutOfRange = 0;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::AdjacentSteps(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::AdjacentSteps(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }
    }

    GIVEN("The class is instantiated with an initial number selection")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                actlib::Numbers::Steps::AdjacentSteps(std::move(generator),
                                                      std::move(range));
            }
        }
    }

    GIVEN("The class is instantiated with an initial number selection")
    {
        int initialSelection = 2; // a mid-range selection (within range)

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(double)));
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(int), 1.0));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        int generatedNumber = 1;
        actlib::Numbers::Steps::AdjacentSteps instance(std::move(generator),
                                                       std::move(range),
                                                       initialSelection);

        WHEN("A number is first requested")
        {
            THEN("The given initial selection is returned and no number is "
                 "generated")
            {
                FORBID_CALL(*generatorPointer, getNumber());
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == initialSelection);
            }

            AND_THEN("The stepwise distribution logic is run, having stripped "
                     "the range offset from the provided initial selection")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(0.0));
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(
                                 (initialSelection - rangePointer->offset) - 1,
                                 1.0));
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(
                                 (initialSelection - rangePointer->offset) + 1,
                                 1.0));
                FORBID_CALL(*generatorPointer,
                            updateDistributionVector(
                                (initialSelection - rangePointer->offset),
                                1.0));

                instance.getNumber();
            }
        }

        WHEN("A subsequent number is requested")
        {
            int generatedNumber = 1;
            instance.getNumber(); // first call

            THEN("It returns a generated number with the range offset added")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .TIMES(1)
                    .RETURN(generatedNumber);

                auto secondCallResult = instance.getNumber(); // subsequent call

                REQUIRE(secondCallResult ==
                        generatedNumber + rangePointer->offset);
            }

            AND_THEN("The stepwise distribution logic is run")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .TIMES(1)
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(0.0))
                    .TIMES(1);
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(ANY(int), 1.0))
                    .TIMES(1);
                instance.getNumber(); // subsequent call
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
                THEN("It returns the initial number provided on instantiation "
                     "and does not call the generator")
                {
                    FORBID_CALL(*generatorPointer, getNumber());
                    instance.getNumber(); // this call makes sure to switch the
                                          // state of m_haveRequestedFirstNumber
                                          // to true

                    instance.reset(); // this resets _haveRequestedFirstNumber
                                      // to false
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber == initialSelection);
                }
            }
        }
    }
}
