#include "Precision.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "NumberProtocolParameters.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Precision: default constructor")
{
    using namespace aleatoric;

    Precision instance(std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto distribution = params.protocols.getPrecision().getDistribution();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(distribution == std::vector<double> {0.5, 0.5});
    }

    THEN("Set is within range")
    {
        for(int i = 0; i < 1000; i++) {
            auto number = instance.getIntegerNumber();
            REQUIRE((number == 0 || number == 1));
        }
    }
}

SCENARIO("Numbers::Precision")
{
    using namespace aleatoric;

    GIVEN("Construction: with an invalid distribution")
    {
        WHEN("The sum of the values in the distribution is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                REQUIRE_THROWS_AS(
                    Precision(std::make_unique<DiscreteGenerator>(),
                              Range(1, 4),
                              std::vector<double> {0.99}),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    Precision(std::make_unique<DiscreteGenerator>(),
                              Range(1, 4),
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
                    Precision(std::make_unique<DiscreteGenerator>(),
                              Range(1, 4),
                              std::vector<double> {1.01}),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    Precision(std::make_unique<DiscreteGenerator>(),
                              Range(1, 4),
                              std::vector<double> {1.01}),
                    "The sum of the values provided as the vector for the "
                    "distribution must equal 1.0");
            }
        }

        WHEN("The size of the distribution vector does not match the size of "
             "the range")
        {
            REQUIRE_THROWS_AS(Precision(std::make_unique<DiscreteGenerator>(),
                                        Range(1, 4),
                                        std::vector<double> {1.0}),
                              std::invalid_argument);

            REQUIRE_THROWS_WITH(
                Precision(std::make_unique<DiscreteGenerator>(),
                          Range(1, 4),
                          std::vector<double> {1.0}),
                "The vector size for the distribution must match the size of "
                "the provided range");
        }
    }

    GIVEN("Construction")
    {
        WHEN("The object is constructed")
        {
            auto generator = std::make_unique<DiscreteGeneratorMock>();
            auto generatorPointer = generator.get();

            Range range(1, 4);

            std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};

            THEN("The generator distribution should have been set with the "
                 "received distribution")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(distribution));

                Precision(std::move(generator), range, distribution);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistributionVector(distribution));

        Range range(1, 4);

        Precision instance(std::move(generator), range, distribution);

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
    }
}

SCENARIO("Numbers:Precision: params")
{
    using namespace aleatoric;

    auto generator = std::make_unique<DiscreteGenerator>();
    auto generatorPointer = generator.get();
    std::vector<double> distribution {0.25, 0.25, 0.25, 0.25};
    Precision instance(std::move(generator), Range(1, 4), distribution);

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("Reflects the state of the object")
        {
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 4);
            REQUIRE(params.protocols.getPrecision().getDistribution() ==
                    distribution);
            REQUIRE(
                params.protocols.getActiveProtocol() ==
                NumberProtocolParameters::Protocols::ActiveProtocol::precision);
        }
    }

    WHEN("Set params")
    {
        Range newRange(11, 15);
        std::vector<double> newDistribution {0.2, 0.2, 0.2, 0.2, 0.2};
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Precision(newDistribution)));
        instance.setParams(newParams);

        THEN("Object state is updated")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();

            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
            REQUIRE(params.protocols.getPrecision().getDistribution() ==
                    newDistribution);
        }

        THEN("Generator is set with new distribution")
        {
            REQUIRE(generatorPointer->getDistributionVector() ==
                    newDistribution);
        }

        THEN("A set of numbers should be from new range")
        {
            std::vector<int> set(1000);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            for(auto &&i : set) {
                REQUIRE(newRange.numberIsInRange(i));
            }
        }
    }

    WHEN("Set params: new distribution does not sum to 1")
    {
        Range newRange(1, 2);

        THEN("Throw exception")
        {
            std::vector<double> newDistribution {0.5, 0.6}; // > 1
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Precision(newDistribution)));
            REQUIRE_THROWS_AS(instance.setParams(newParams),
                              std::invalid_argument);
        }

        THEN("Throw exception")
        {
            std::vector<double> newDistribution {0.5, 0.4}; // < 1
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Precision(newDistribution)));
            REQUIRE_THROWS_AS(instance.setParams(newParams),
                              std::invalid_argument);
        }
    }

    WHEN("Set params: new distribution and new range sizes do not match")
    {
        Range newRange(1, 2);
        std::vector<double> newDistribution {1};
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Precision(newDistribution)));

        THEN("Throw exception")
        {
            REQUIRE_THROWS_AS(instance.setParams(newParams),
                              std::invalid_argument);
        }
    }
}
