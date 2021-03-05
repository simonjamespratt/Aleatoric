#include "Subset.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>

SCENARIO("Numbers::Subset: default constructor")
{
    using namespace aleatoric;

    Subset instance(std::make_unique<UniformGenerator>(),
                    std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto subsetParams = params.protocols.getSubset();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE(subsetParams.getMin() == 1);
        REQUIRE(subsetParams.getMax() == 2);
    }

    THEN("Possible unique values in a set are as expected")
    {
        std::vector<std::vector<int>> possibleUniqueValues {{0}, {1}, {0, 1}};
        std::vector<int> set(1000);
        for(auto &&i : set) {
            i = instance.getIntegerNumber();
        }

        auto uniqueValues = set;
        std::sort(uniqueValues.begin(), uniqueValues.end());
        auto last = std::unique(uniqueValues.begin(), uniqueValues.end());
        uniqueValues.erase(last,
                           uniqueValues.end()); // removes spaces previously
                                                // taken up by duplicates
        REQUIRE_THAT(uniqueValues,
                     Catch::Equals(possibleUniqueValues[0]) ||
                         Catch::Equals(possibleUniqueValues[1]) ||
                         Catch::Equals(possibleUniqueValues[2]));
    }
}

SCENARIO("Numbers::Subset")
{
    using namespace aleatoric;

    GIVEN("Construction: with invalid subset parameters")
    {
        WHEN("Min is less than 1")
        {
            THEN("Throw execption")
            {
                int invalidSubsetMin = 0;

                REQUIRE_THROWS_AS(Subset(std::make_unique<UniformGenerator>(),
                                         std::make_unique<DiscreteGenerator>(),
                                         Range(0, 9),
                                         invalidSubsetMin,
                                         9),
                                  std::invalid_argument);
            }
        }

        WHEN("Min is greater than max")
        {
            THEN("Throw execption")
            {
                int invalidMin = 7;
                int invalidMax = 6;

                REQUIRE_THROWS_AS(Subset(std::make_unique<UniformGenerator>(),
                                         std::make_unique<DiscreteGenerator>(),
                                         Range(0, 9),
                                         invalidMin,
                                         invalidMax),
                                  std::invalid_argument);
            }
        }

        WHEN("Max is greater than the range size")
        {
            THEN("Throw execption")
            {
                int invalidSubsetMax = 11;

                REQUIRE_THROWS_AS(Subset(std::make_unique<UniformGenerator>(),
                                         std::make_unique<DiscreteGenerator>(),
                                         Range(0, 9),
                                         1,
                                         invalidSubsetMax),
                                  std::invalid_argument);
            }
        }
    }

    GIVEN("Construction")
    {
        int subSetMin = 4;
        int subSetMax = 7;

        WHEN("In the constructor")
        {
            THEN("A subset size should have been chosen that is within the "
                 "subset min and max values received (inclusive)")
            {
                auto uniformGenerator =
                    std::make_unique<UniformGeneratorMock>();
                auto uniformGeneratorPointer = uniformGenerator.get();
                ALLOW_CALL(*uniformGeneratorPointer,
                           setDistribution(ANY(int), ANY(int)));

                Range range(1, 10);

                REQUIRE_CALL(*uniformGeneratorPointer,
                             setDistribution(subSetMin, subSetMax));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber()).RETURN(1);

                Subset(std::move(uniformGenerator),
                       std::make_unique<DiscreteGenerator>(),
                       range,
                       subSetMin,
                       subSetMax);
            }

            THEN("The subset collection should be filled with non-repeated "
                 "numbers from the range supplied")
            {
                // NB: the offset is added to each element in the subset here,
                // but is not testable until the point at which a number is
                // requested - see below

                const int selectedSubsetSize = 5;
                int numberSelectedByDiscreteGenerator = 1;

                Range range(1, 10);

                auto uniformGenerator =
                    std::make_unique<UniformGeneratorMock>();
                auto uniformGeneratorPointer = uniformGenerator.get();

                ALLOW_CALL(*uniformGeneratorPointer,
                           setDistribution(ANY(int), ANY(int)));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber())
                    .RETURN(selectedSubsetSize);

                auto discreteGenerator =
                    std::make_unique<DiscreteGeneratorMock>();
                auto discreteGeneratorPointer = discreteGenerator.get();

                REQUIRE_CALL(*discreteGeneratorPointer,
                             setDistributionVector(range.size, 1.0));

                REQUIRE_CALL(*discreteGeneratorPointer, getNumber())
                    .TIMES(selectedSubsetSize)
                    .RETURN(numberSelectedByDiscreteGenerator);

                REQUIRE_CALL(
                    *discreteGeneratorPointer,
                    updateDistributionVector(numberSelectedByDiscreteGenerator,
                                             0.0))
                    .TIMES(selectedSubsetSize);

                Subset(std::move(uniformGenerator),
                       std::move(discreteGenerator),
                       range,
                       subSetMin,
                       subSetMax);
            }

            THEN("The uniform generator should be set for selecting indices "
                 "from the subset in readiness for when a number is requested")
            {
                const int selectedSubsetSize = 5;

                auto uniformGenerator =
                    std::make_unique<UniformGeneratorMock>();
                auto uniformGeneratorPointer = uniformGenerator.get();

                Range range(1, 10);

                ALLOW_CALL(*uniformGeneratorPointer,
                           setDistribution(ANY(int), ANY(int)));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber())
                    .RETURN(selectedSubsetSize);

                REQUIRE_CALL(*uniformGeneratorPointer,
                             setDistribution(0, selectedSubsetSize - 1));

                Subset(std::move(uniformGenerator),
                       std::make_unique<DiscreteGenerator>(),
                       range,
                       subSetMin,
                       subSetMax);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        WHEN("A number is requested")
        {
            int subSetMin = 4;
            int subSetMax = 7;
            const int selectedSubsetSize = 5;
            int numberSelectedByDiscreteGenerator = 1;

            Range range(1, 10);

            auto uniformGenerator = std::make_unique<UniformGeneratorMock>();
            auto uniformGeneratorPointer = uniformGenerator.get();

            ALLOW_CALL(*uniformGeneratorPointer,
                       setDistribution(ANY(int), ANY(int)));

            REQUIRE_CALL(*uniformGeneratorPointer, getNumber())
                .RETURN(selectedSubsetSize);

            auto discreteGenerator = std::make_unique<DiscreteGeneratorMock>();
            auto discreteGeneratorPointer = discreteGenerator.get();

            ALLOW_CALL(*discreteGeneratorPointer,
                       setDistributionVector(range.size, 1.0));

            ALLOW_CALL(*discreteGeneratorPointer, getNumber())
                .RETURN(numberSelectedByDiscreteGenerator);

            ALLOW_CALL(
                *discreteGeneratorPointer,
                updateDistributionVector(numberSelectedByDiscreteGenerator,
                                         0.0));

            Subset instance(std::move(uniformGenerator),
                            std::move(discreteGenerator),
                            range,
                            subSetMin,
                            subSetMax);

            THEN("It should select an item from the subset collection "
                 "using a generated number as the index to select")
            {
                REQUIRE_CALL(*uniformGeneratorPointer, getNumber()).RETURN(1);

                instance.getIntegerNumber();
            }

            THEN("The number returned should be the result of the number "
                 "generated by the discrete generator in filling the subset "
                 "with the range offset added")
            {
                // NB: this is testing something that actually happens in the
                // constructor but is only testable when a number is requested

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber()).RETURN(1);

                auto returnedNumber = instance.getIntegerNumber();

                REQUIRE(returnedNumber ==
                        numberSelectedByDiscreteGenerator + range.offset);
            }
        }
    }
}

SCENARIO("Numbers::subset: params")
{
    using namespace aleatoric;

    int subsetMin = 2;
    int subsetMax = 8;

    Subset instance(std::make_unique<UniformGenerator>(),
                    std::make_unique<DiscreteGenerator>(),
                    Range(1, 10),
                    subsetMin,
                    subsetMax);

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("Reflects object state")
        {
            auto subsetParams = params.protocols.getSubset();
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 10);
            REQUIRE(subsetParams.getMin() == subsetMin);
            REQUIRE(subsetParams.getMax() == subsetMax);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocol::Type::subset);
        }
    }

    WHEN("Set params")
    {
        Range newRange(20, 30);
        int newMin = 3;
        int newMax = 5;
        NumberProtocolConfig newParams(
            newRange,
            NumberProtocolParams(SubsetParams(newMin, newMax)));
        instance.setParams(newParams);

        auto params = instance.getParams();
        auto returnedRange = params.getRange();
        auto subsetParams = params.protocols.getSubset();

        THEN("Object state is updated")
        {
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
            REQUIRE(subsetParams.getMin() == newMin);
            REQUIRE(subsetParams.getMax() == newMax);
        }

        WHEN("Output")
        {
            std::vector<int> set(1000);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("All numbers should be in new range")
            {
                for(auto &&i : set) {
                    REQUIRE(newRange.numberIsInRange(i));
                }
            }

            THEN("number of unique values is between subset min max")
            {
                auto uniqueValues = set;
                std::sort(uniqueValues.begin(), uniqueValues.end());
                auto last =
                    std::unique(uniqueValues.begin(), uniqueValues.end());
                uniqueValues.erase(
                    last,
                    uniqueValues.end()); // removes spaces previously taken up
                                         // by duplicates
                REQUIRE(uniqueValues.size() >= newMin);
                REQUIRE(uniqueValues.size() <= newMax);
            }
        }
    }

    WHEN("Set params: invalid params")
    {
        WHEN("Min is less than 1")
        {
            int newMin = 0;
            int newMax = 7;
            NumberProtocolConfig newParams(
                Range(20, 30),
                NumberProtocolParams(SubsetParams(newMin, newMax)));

            THEN("Throw execption")
            {
                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }

        WHEN("Min is greater than max")
        {
            int newMin = 8;
            int newMax = 7;
            NumberProtocolConfig newParams(
                Range(20, 30),
                NumberProtocolParams(SubsetParams(newMin, newMax)));

            THEN("Throw execption")
            {
                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }

        WHEN("Max is greater than the range size")
        {
            Range newRange(20, 30);
            int newMin = 8;
            int newMax = newRange.size + 1;
            NumberProtocolConfig newParams(
                newRange,
                NumberProtocolParams(SubsetParams(newMin, newMax)));

            THEN("Throw execption")
            {
                REQUIRE_THROWS_AS(instance.setParams(newParams),
                                  std::invalid_argument);
            }
        }
    }
}
