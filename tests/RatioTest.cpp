#include "Ratio.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Ratio: default constructor")
{
    using namespace aleatoric;

    Ratio instance(std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto ratios = params.protocols.getRatio().getRatios();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(ratios == std::vector<int> {1, 1});
    }

    THEN("Set of numbers is as per basic serial process")
    {
        // Default ratio settings makes it function as though it were a basic
        // serial process

        std::vector<std::vector<int>> possibleResults {{0, 1}, {1, 0}};
        std::vector<std::vector<int>> set;

        for(int i = 0; i < 1000; i++) {
            std::vector<int> pair(2);
            for(auto &&i : pair) {
                i = instance.getIntegerNumber();
            }
            set.push_back(pair);
        }

        for(auto &&pair : set) {
            REQUIRE_THAT(pair,
                         Catch::Equals(possibleResults[0]) ||
                             Catch::Equals(possibleResults[1]));
        }
    }
}

SCENARIO("Numbers::Ratio")
{
    using namespace aleatoric;

    GIVEN("Construction: with mismatched range and ratios collection")
    {
        WHEN("The constructor is called")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                // should have a size of 3 if it were to match the range size
                std::vector<int> ratios {1, 2};

                REQUIRE_THROWS_AS(Ratio(std::make_unique<DiscreteGenerator>(),
                                        Range(10, 12),
                                        ratios),
                                  std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    Ratio(std::make_unique<DiscreteGenerator>(),
                          Range(10, 12),
                          ratios),
                    "The size of ratios collection must match the size of the "
                    "range");
            }
        }
    }

    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        // 3 numbers because the range is inclusive
        std::vector<int> ratios {1, 2, 3};
        int ratiosSum = 6;

        WHEN("The constructor is called")
        {
            THEN("The generator should be set to the size of the sum of the "
                 "ratios supplied with equal probability")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(ratiosSum, 1.0));
                Ratio(std::move(generator), Range(10, 12), ratios);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(1);
        ALLOW_CALL(*generatorPointer,
                   setDistributionVector(ANY(int), ANY(double)));
        ALLOW_CALL(*generatorPointer,
                   updateDistributionVector(ANY(int), ANY(double)));
        // ensures that seriesIsComplete returns true
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        Range range(10, 12);

        // 3 numbers because the range is inclusive
        std::vector<int> ratios {1, 3, 5};

        Ratio instance(std::move(generator), range, ratios);

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(1);
                instance.getIntegerNumber();
            }

            THEN("It updates the generator to disallow the selected number "
                 "from being selected in future calls")
            {
                int generatedNumber = 1;

                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);

                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));

                instance.getIntegerNumber();
            }

            THEN("It returns the number held within the selectables collection "
                 "at the index specified by the generated number")
            {
                std::vector<int>
                    expectedSelectables {10, 11, 11, 11, 12, 12, 12, 12, 12};

                // get generator to return numbers for each index in
                // expectedSelectables
                for(int i = 0; i < expectedSelectables.size(); i++) {
                    REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(i);

                    auto returnedNumber = instance.getIntegerNumber();

                    REQUIRE(returnedNumber == expectedSelectables[i]);
                }
            }

            THEN("It gets the state of the generator distribution for "
                 "determining if the series is complete")
            {
                // NB: the return value for this test can be anything
                REQUIRE_CALL(*generatorPointer, getDistributionVector())
                    .RETURN(std::vector<double> {1.0});
                instance.getIntegerNumber();
            }

            AND_WHEN("The series is complete")
            {
                THEN("It resets the generator distribution")
                {
                    REQUIRE_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {0.0});

                    REQUIRE_CALL(*generatorPointer,
                                 updateDistributionVector(1.0));

                    instance.getIntegerNumber();
                }
            }

            AND_WHEN("The series is not complete")
            {
                THEN("It does not reset the generator distribution")
                {
                    REQUIRE_CALL(*generatorPointer, getDistributionVector())
                        .RETURN(std::vector<double> {1.0});

                    FORBID_CALL(*generatorPointer,
                                updateDistributionVector(1.0));

                    instance.getIntegerNumber();
                }
            }
        }
    }
}

SCENARIO("Numbers::Ratio: params")
{
    using namespace aleatoric;

    std::vector<int> ratios {1, 1, 1};
    Ratio instance(std::make_unique<DiscreteGenerator>(), Range(1, 3), ratios);

    std::vector<int> initialExpectedSelectables {1, 2, 3};

    std::vector<int> initialSet(initialExpectedSelectables.size());
    for(auto &&i : initialSet) {
        i = instance.getIntegerNumber();
    }

    REQUIRE_THAT(initialSet,
                 Catch::UnorderedEquals(initialExpectedSelectables));

    WHEN("get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("Reflects object state")
        {
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 3);
            REQUIRE(params.protocols.getRatio().getRatios() == ratios);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocol::Type::ratio);
        }
    }

    WHEN("set params")
    {
        Range newRange(4, 6);
        std::vector<int> newRatios {1, 2, 3};
        NumberProtocolConfig newParams(
            newRange,
            NumberProtocolParams(RatioParams(newRatios)));
        instance.setParams(newParams);

        THEN("Object is updated")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();

            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
            REQUIRE(params.protocols.getRatio().getRatios() == newRatios);
        }

        THEN("Output is as expected")
        {
            std::vector<int> updatedExpectedSelectables {4, 5, 5, 6, 6, 6};

            std::vector<int> newSet(updatedExpectedSelectables.size());
            for(auto &&i : newSet) {
                i = instance.getIntegerNumber();
            }

            REQUIRE_THAT(newSet,
                         Catch::UnorderedEquals(updatedExpectedSelectables));
        }
    }

    WHEN("set params: ratios and range sizes do not match")
    {
        Range newRange(4, 6);
        std::vector<int> newRatios {2, 3};
        NumberProtocolConfig newParams(
            newRange,
            NumberProtocolParams(RatioParams(newRatios)));

        THEN("Throw exception")
        {
            REQUIRE_THROWS_AS(instance.setParams(newParams),
                              std::invalid_argument);
        }
    }
}
