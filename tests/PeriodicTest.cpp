#include "Periodic.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Periodic")
{
    GIVEN("The class is instantiated with an invalid chanceOfRepetition value")
    {
        WHEN("The value provided is greater than 1.0")
        {
            THEN("A standard invalid_argument exeception is thrown")
            {
                double invalidChanceValue = 1.1;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        invalidChanceValue),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
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
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        invalidChanceValue),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        invalidChanceValue),
                    "The value passed as argument for chanceOfRepetition must "
                    "be within the range of 0.0 - 1.0");
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(initialGeneratorDistributionState);

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                actlib::Numbers::Steps::Periodic(std::move(generator),
                                                 std::move(range),
                                                 0.5);
            }
        }
    }

    GIVEN("The class is instantiated without an initial number selection")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};

        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(initialGeneratorDistributionState);
        ALLOW_CALL(*generatorPointer,
                   setDistributionVector(ANY(std::vector<double>)));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        double chanceOfRepetition = 0.5;

        actlib::Numbers::Steps::Periodic instance(std::move(generator),
                                                  std::move(range),
                                                  chanceOfRepetition);

        int generatedNumber = 1; // mid range selection

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
                instance.getNumber();
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
    }

    GIVEN("The class is instantiated with an invalid initialSelection value")
    {
        WHEN("The value provided is greater than the range end")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidInitialSelection = 4;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        0.5,
                        invalidInitialSelection),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        0.5,
                        invalidInitialSelection),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }

        WHEN("The value provided is less than the range start")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidInitialSelection = 0;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        0.5,
                        invalidInitialSelection),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::Periodic(
                        std::make_unique<actlib::Numbers::DiscreteGenerator>(),
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        0.5,
                        invalidInitialSelection),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }
    }

    GIVEN("The class is instantiated with a valid initial number selection")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(initialGeneratorDistributionState);

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                actlib::Numbers::Steps::Periodic(std::move(generator),
                                                 std::move(range),
                                                 0.5,
                                                 2);
            }
        }
    }

    GIVEN("The class is instantiated with a valid initial number selection")
    {
        double chanceOfRepetition = 0.5;
        int initialSelection = 1; // start of range selection

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        std::vector<double> initialGeneratorDistributionState {1.0, 1.0, 1.0};

        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(initialGeneratorDistributionState);
        ALLOW_CALL(*generatorPointer,
                   setDistributionVector(ANY(std::vector<double>)));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
        auto rangePointer = range.get();

        actlib::Numbers::Steps::Periodic instance(std::move(generator),
                                                  std::move(range),
                                                  chanceOfRepetition,
                                                  initialSelection);

        WHEN("The first number is requested")
        {
            THEN("The initial number selection is returned")
            {
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber == initialSelection);
            }

            THEN("The generator should not have been called to get a number")
            {
                FORBID_CALL(*generatorPointer, getNumber());
                instance.getNumber();
            }

            AND_THEN("The generator distribution should be set with a bias in "
                     "favour of the initial number selection according to the "
                     "periodicity received during construction")
            {
                REQUIRE_CALL(*generatorPointer, getDistributionVector())
                    .RETURN(initialGeneratorDistributionState);

                // The index in the distribution vector for the initial
                // selection is equal to the initial selection, LESS, the range
                // offset
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(
                                 std::vector<double> {0.5, 0.25, 0.25}));
                instance.getNumber();
            }
        }

        WHEN("A subsequent number is requested")
        {
            instance.getNumber();    // initial call to get number
            int generatedNumber = 2; // end of range selection
            THEN("The generated number is returned with the range offset "
                 "added")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getNumber();
                REQUIRE(returnedNumber ==
                        generatedNumber + rangePointer->offset);
            }

            AND_THEN("The generator distribution is set in favour of this "
                     "generated number, according to the periodicity received "
                     "during construction")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {0.5, 0.25, 0.25});
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(
                                 std::vector<double> {0.25, 0.25, 0.5}));
                instance.getNumber();
            }
        }

        WHEN("A reset is performed following a previous call to get a number")
        {
            ALLOW_CALL(*generatorPointer, updateDistributionVector(1.0));

            instance.getNumber(); // unsets the initial state

            AND_WHEN("The next number is requested")
            {
                THEN("The initial selection is returned, due to the initial "
                     "state having been reinstated during the reset")
                {
                    instance.reset();
                    auto returnedNumber = instance.getNumber();
                    REQUIRE(returnedNumber == initialSelection);
                }
            }
        }
    }
}
