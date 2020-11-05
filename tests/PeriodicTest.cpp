#include "Periodic.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Periodic: default constructor")
{
    using namespace aleatoric;

    Periodic instance(std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto chanceOfRepetition =
            params.protocols.getPeriodic().getChanceOfRepetition();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(chanceOfRepetition == 0.0);
    }

    std::vector<int> set(1000);
    for(auto &&i : set) {
        i = instance.getIntegerNumber();
    }

    THEN("Set is within range")
    {
        for(auto &&i : set) {
            REQUIRE((i == 0 | i == 1));
        }
    }

    THEN("There is no direct repetition within set")
    {
        for(auto i = set.begin(); i != set.end(); ++i) {
            // Don't make assertion for first number in collection as
            // there is no previous number to compare with
            if(i != set.begin()) {
                REQUIRE(*i != *std::prev(i));
            }
        }
    }
}

SCENARIO("Numbers::Periodic")
{
    using namespace aleatoric;

    GIVEN("Construction: with an invalid chanceOfRepetition value")
    {
        WHEN("The value provided is greater than 1.0")
        {
            THEN("A standard invalid_argument exeception is thrown")
            {
                double invalidChanceValue = 1.1;

                REQUIRE_THROWS_AS(
                    Periodic(std::make_unique<DiscreteGenerator>(),
                             Range(1, 3),
                             invalidChanceValue),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    Periodic(std::make_unique<DiscreteGenerator>(),
                             Range(1, 3),
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
                    Periodic(std::make_unique<DiscreteGenerator>(),
                             Range(1, 3),
                             invalidChanceValue),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    Periodic(std::make_unique<DiscreteGenerator>(),
                             Range(1, 3),
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

        Range range(1, 3);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(range.size, 1.0));
                Periodic(std::move(generator), range, 0.5);
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

        Range range(1, 3);

        double chanceOfRepetition = 0.5;

        Periodic instance(std::move(generator), range, chanceOfRepetition);

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
    }
}

SCENARIO("Numbers::Periodic: params")
{
    using namespace aleatoric;

    Range range(1, 10);
    double chanceOfRepetition = 0.5;
    auto generator = std::make_unique<DiscreteGenerator>();
    auto generatorPointer = generator.get();
    Periodic instance(std::move(generator), range, chanceOfRepetition);

    WHEN("Get params")
    {
        THEN("They should match the state of the object")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();

            REQUIRE(returnedRange.start == range.start);
            REQUIRE(returnedRange.end == range.end);

            REQUIRE(params.protocols.getPeriodic().getChanceOfRepetition() ==
                    0.5);
            REQUIRE(
                params.protocols.getActiveProtocol() ==
                NumberProtocolParameters::Protocols::ActiveProtocol::periodic);
        }
    }

    WHEN("Set params: Periodicity")
    {
        AND_WHEN("The value is too small")
        {
            THEN("An exception is thrown")
            {
                double invalidPeriodicity = -0.1;
                NumberProtocolParameters newParams(
                    Range(0, 1),
                    NumberProtocolParameters::Protocols(
                        NumberProtocolParameters::Periodic(
                            invalidPeriodicity)));

                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }

        AND_WHEN("The value is too large")
        {
            THEN("An exception is thrown")
            {
                double invalidPeriodicity = 1.1;
                NumberProtocolParameters newParams(
                    Range(0, 1),
                    NumberProtocolParameters::Protocols(
                        NumberProtocolParameters::Periodic(
                            invalidPeriodicity)));

                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }

        AND_WHEN("The value is valid")
        {
            double newPeriodicity = 1.0;
            NumberProtocolParameters newParams(
                Range(0, 1),
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Periodic(newPeriodicity)));
            instance.setParams(newParams);

            THEN("The object state is updated")
            {
                auto params = instance.getParams();
                REQUIRE(
                    params.protocols.getPeriodic().getChanceOfRepetition() ==
                    newPeriodicity);
            }

            AND_WHEN("A set of numbers is gathered")
            {
                std::vector<int> set(100);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                THEN("The values should be as expected")
                {
                    auto firstNumber = set.front();

                    for(auto &&i : set) {
                        REQUIRE(i == firstNumber);
                    }
                }
            }
        }
    }

    WHEN("Set params: Range")
    {
        THEN("The object state is updated")
        {
            Range newRange(11, 20);
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Periodic(chanceOfRepetition)));

            instance.setParams(newParams);
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }

        AND_WHEN("No numbers have been returned yet")
        {
            Range newRange(11, 13); // was (1, 10)
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Periodic(chanceOfRepetition)));

            instance.setParams(newParams);

            THEN("The generator distribution should be equal "
                 "probability for the new range size")
            {
                std::vector<double> expectedDistribution {1, 1, 1};
                REQUIRE(generatorPointer->getDistributionVector() ==
                        expectedDistribution);
            }
        }

        AND_WHEN("The last returned number is outside the new range")
        {
            auto lastNumber = instance.getIntegerNumber();
            Range newRange(lastNumber + 1,
                           lastNumber + 3); // size of 3
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Periodic(chanceOfRepetition)));

            instance.setParams(newParams);

            THEN("The generator distribution should be equal "
                 "probability for the new range size")
            {
                std::vector<double> expectedDistribution {1, 1, 1};
                REQUIRE(generatorPointer->getDistributionVector() ==
                        expectedDistribution);
            }
        }

        AND_WHEN("The last returned number is within the new range")
        {
            auto lastNumber = instance.getIntegerNumber();
            Range newRange(lastNumber - 2,
                           lastNumber + 2); // size of 5
            double newChanceOfRepetition = 0.25;

            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Periodic(newChanceOfRepetition)));

            instance.setParams(newParams);

            THEN("The generator distribution should favour the last "
                 "number returned with the new chance of repetition")
            {
                // The total of all values in the vector must equal 1.0.
                // The
                // value at the index of the last selected number must
                // have the value of the periodicity
                // (chanceOfRepetition). The remainder of 1.0 -
                // periodicity is shared equally amongst the remaining
                // vector indices.

                std::vector<double> expectedDistribution {0.1875,
                                                          0.1875,
                                                          newChanceOfRepetition,
                                                          0.1875,
                                                          0.1875};

                REQUIRE(generatorPointer->getDistributionVector() ==
                        expectedDistribution);
            }
        }
    }
}
