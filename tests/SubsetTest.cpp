#include "Subset.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>

SCENARIO("Numbers::Subset")
{
    GIVEN("Construction: with invalid subset parameters")
    {
        WHEN("The subset min is less than 1")
        {
            THEN("A standard invalid_argument execption is thrown")
            {
                int invalidSubsetMin = 0;

                REQUIRE_THROWS_AS(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        invalidSubsetMin,
                        10),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        invalidSubsetMin,
                        10),
                    "The value passed as argument for subsetMin must be "
                    "between 1 and the range size, inclusive");
            }
        }

        WHEN("The subset min is greater than the range size")
        {
            THEN("A standard invalid_argument execption is thrown")
            {
                int invalidSubsetMin = 11;

                REQUIRE_THROWS_AS(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        invalidSubsetMin,
                        10),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        invalidSubsetMin,
                        10),
                    "The value passed as argument for subsetMin must be "
                    "between 1 and the range size, inclusive");
            }
        }

        WHEN("The subset max is less than 1")
        {
            THEN("A standard invalid_argument execption is thrown")
            {
                int invalidSubsetMax = 0;

                REQUIRE_THROWS_AS(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        1,
                        invalidSubsetMax),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        1,
                        invalidSubsetMax),
                    "The value passed as argument for subsetMax must be "
                    "between 1 and the range size, inclusive");
            }
        }

        WHEN("The subset max is greater than the range size")
        {
            THEN("A standard invalid_argument execption is thrown")
            {
                int invalidSubsetMax = 11;

                REQUIRE_THROWS_AS(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        1,
                        invalidSubsetMax),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        1,
                        invalidSubsetMax),
                    "The value passed as argument for subsetMax must be "
                    "between 1 and the range size, inclusive");
            }
        }

        WHEN("The subset min is greater than the subset max")
        {
            THEN("A standard invalid_argument execption is thrown")
            {
                int invalidSubsetMin = 2;
                int invalidSubsetMax = 1;

                REQUIRE_THROWS_AS(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        invalidSubsetMin,
                        invalidSubsetMax),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Subset(
                        std::make_unique<aleatoric::UniformGenerator>(),
                        std::make_unique<aleatoric::DiscreteGenerator>(),
                        aleatoric::Range(0, 9),
                        invalidSubsetMin,
                        invalidSubsetMax),
                    "The value passed for subsetMin may not be greater than "
                    "that of subsetMax");
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

                aleatoric::Range range(1, 10);

                REQUIRE_CALL(*uniformGeneratorPointer,
                             setDistribution(subSetMin, subSetMax));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber()).RETURN(1);

                aleatoric::Subset(
                    std::move(uniformGenerator),
                    std::make_unique<aleatoric::DiscreteGenerator>(),
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

                aleatoric::Range range(1, 10);

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

                aleatoric::Subset(std::move(uniformGenerator),
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

                aleatoric::Range range(1, 10);

                ALLOW_CALL(*uniformGeneratorPointer,
                           setDistribution(ANY(int), ANY(int)));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber())
                    .RETURN(selectedSubsetSize);

                REQUIRE_CALL(*uniformGeneratorPointer,
                             setDistribution(0, selectedSubsetSize - 1));

                aleatoric::Subset(
                    std::move(uniformGenerator),
                    std::make_unique<aleatoric::DiscreteGenerator>(),
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

            aleatoric::Range range(1, 10);

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

            aleatoric::Subset instance(std::move(uniformGenerator),
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

        WHEN("A reset is requested")
        {
            int subSetMin = 4;
            int subSetMax = 7;

            aleatoric::Range range(1, 10);

            auto uniformGenerator = std::make_unique<UniformGeneratorMock>();
            auto uniformGeneratorPointer = uniformGenerator.get();

            ALLOW_CALL(*uniformGeneratorPointer,
                       setDistribution(ANY(int), ANY(int)));
            ALLOW_CALL(*uniformGeneratorPointer, getNumber()).RETURN(1);

            auto discreteGenerator = std::make_unique<DiscreteGeneratorMock>();
            auto discreteGeneratorPointer = discreteGenerator.get();

            ALLOW_CALL(*discreteGeneratorPointer,
                       setDistributionVector(range.size, 1.0));

            ALLOW_CALL(*discreteGeneratorPointer, getNumber()).RETURN(1);

            ALLOW_CALL(*discreteGeneratorPointer,
                       updateDistributionVector(ANY(int), ANY(double)));

            aleatoric::Subset instance(std::move(uniformGenerator),
                                       std::move(discreteGenerator),
                                       range,
                                       subSetMin,
                                       subSetMax);

            THEN("A subset size should have been chosen that is within the "
                 "subset min and max values received (inclusive)")
            {
                FORBID_CALL(*discreteGeneratorPointer,
                            setDistributionVector(ANY(int), ANY(double)));

                REQUIRE_CALL(*uniformGeneratorPointer,
                             setDistribution(subSetMin, subSetMax));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber()).RETURN(1);

                instance.reset();
            }

            THEN("The subset collection should be filled with non-repeated "
                 "numbers from the range supplied")
            {
                const int selectedSubsetSize = 5;
                int numberSelectedByDiscreteGenerator = 1;

                FORBID_CALL(*discreteGeneratorPointer,
                            setDistributionVector(ANY(int), ANY(double)));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber())
                    .RETURN(selectedSubsetSize);

                REQUIRE_CALL(*discreteGeneratorPointer, getNumber())
                    .TIMES(selectedSubsetSize)
                    .RETURN(numberSelectedByDiscreteGenerator);

                REQUIRE_CALL(
                    *discreteGeneratorPointer,
                    updateDistributionVector(numberSelectedByDiscreteGenerator,
                                             0.0))
                    .TIMES(selectedSubsetSize);

                instance.reset();
            }

            THEN("The uniform generator should be set for selecting indices "
                 "from the subset in readiness for when a number is requested")
            {
                const int selectedSubsetSize = 5;

                FORBID_CALL(*discreteGeneratorPointer,
                            setDistributionVector(ANY(int), ANY(double)));

                REQUIRE_CALL(*uniformGeneratorPointer, getNumber())
                    .RETURN(selectedSubsetSize);

                REQUIRE_CALL(*uniformGeneratorPointer,
                             setDistribution(0, selectedSubsetSize - 1));

                instance.reset();
            }
        }
    }

    GIVEN("The object is constructed (with real dependencies")
    {
        aleatoric::Subset realInstance(
            std::make_unique<aleatoric::UniformGenerator>(),
            std::make_unique<aleatoric::DiscreteGenerator>(),
            aleatoric::Range(1, 10),
            3,
            5);

        WHEN("The range is changed")
        {
            aleatoric::Range newRange(11, 22);
            realInstance.setRange(newRange);

            THEN("The returned range should match the one received")
            {
                auto returnedRange = realInstance.getRange();
                REQUIRE(returnedRange.start == newRange.start);
                REQUIRE(returnedRange.end == newRange.end);
            }

            AND_WHEN("A set of numbers is requested")
            {
                std::vector<int> set(100);
                for(auto &&i : set) {
                    i = realInstance.getIntegerNumber();
                }

                THEN("The set contains only numbers from the new range")
                {
                    for(auto &&i : set) {
                        REQUIRE(i >= newRange.start);
                        REQUIRE(i <= newRange.end);
                    }
                }
            }
        }
    }
}
