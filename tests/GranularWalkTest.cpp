#include "GranularWalk.hpp"

#include "NumberProtocolParameters.hpp"
#include "Range.hpp"
#include "UniformRealGenerator.hpp"

#include <catch2/catch.hpp>

SCENARIO("Numbers::GranularWalk: default constructor")
{
    using namespace aleatoric;

    GranularWalk instance(std::make_unique<UniformRealGenerator>());

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

SCENARIO("Numbers::GranularWalk: argument validity")
{
    using namespace aleatoric;

    GIVEN("Construction: with an invalid deviation factor")
    {
        WHEN("The value provided is less than 0.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                double invalidDeviation = -0.01;

                REQUIRE_THROWS_AS(
                    GranularWalk(std::make_unique<UniformRealGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    GranularWalk(std::make_unique<UniformRealGenerator>(),
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
                    GranularWalk(std::make_unique<UniformRealGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    GranularWalk(std::make_unique<UniformRealGenerator>(),
                                 Range(10, 20),
                                 invalidDeviation),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }
    }
}

SCENARIO("Numbers::GranularWalk: basic functionality")
{
    using namespace aleatoric;

    auto generator = std::make_unique<UniformRealGenerator>();
    auto generatorPointer = generator.get();
    Range range(10, 20);
    double deviationFactor = 0.1;
    double maxStep = (range.end - range.start) * deviationFactor; // 1

    GranularWalk instance(std::move(generator), range, deviationFactor);

    THEN("Generator distribution is set to full range on construction")
    {
        auto distribution = generatorPointer->getDistribution();
        REQUIRE(distribution.first == range.start);
        REQUIRE(distribution.second == range.end);
    }

    WHEN("Test number production")
    {
        std::vector<double> set(10000);
        for(auto &&i : set) {
            i = instance.getDecimalNumber();

            std::vector<std::pair<double, double>> possibleResults {
                std::make_pair(range.start, (i + maxStep)),
                std::make_pair((i - maxStep), range.end),
                std::make_pair((i - maxStep), (i + maxStep))};

            // NB: check that the generator is being set for next step correctly
            REQUIRE_THAT(
                possibleResults,
                Catch::VectorContains(generatorPointer->getDistribution()));
        }

        THEN("Numbers are within given range")
        {
            for(auto &&i : set) {
                REQUIRE(i >= range.start);
                REQUIRE(i <= range.end);
            }
        }

        THEN("Numbers are within the max step of the last number")
        {
            std::vector<double> differences(set.size());
            std::adjacent_difference(set.begin(),
                                     set.end(),
                                     differences.begin());

            //  remove the first element as it is the value of the first
            //  element in the set (and not a difference between two
            //  numbers in the set)
            differences.erase(differences.begin());

            for(auto &&i : differences) {
                REQUIRE(i <= maxStep);
            }
        }

        THEN("A walk from the starting point should take place")
        {
            // A coarse approach to testing that in large part, there are
            // changes of number (thus creating a sense of a walk) within the
            // number set

            // Can't think of a more refined (nor accurate) way of testing this

            int numberChangeCount = 0;
            for(auto i = set.begin(); i != set.end(); ++i) {
                // Don't consider first number in collection as
                // there is no previous number to compare with
                if(i != set.begin()) {
                    auto current = *i;
                    auto prev = *std::prev(i);
                    if(current != prev) {
                        numberChangeCount++;
                    }
                }
            }
            // for set of 10000 numbers this should be approx 5, so 10 is
            // generous
            REQUIRE_THAT(numberChangeCount, Catch::WithinAbs(10000, 10));
        }
    }

    WHEN("resetting")
    {
        auto number = instance.getDecimalNumber();
        auto dist = generatorPointer->getDistribution();
        REQUIRE((dist.first != range.start || dist.second != range.end));

        THEN("generator should be set to full range")
        {
            instance.reset();
            auto dist = generatorPointer->getDistribution();
            REQUIRE(dist.first == range.start);
            REQUIRE(dist.second == range.end);
        }
    }
}

SCENARIO("Numbers::GranularWalk: params")
{
    using namespace aleatoric;

    GranularWalk instance(std::make_unique<UniformRealGenerator>(),
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
