#include "Ratio.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Ratio")
{
    GIVEN("The object is being constructed with a range and ratios collections "
          "whose sizes do not match")
    {
        WHEN("The constructor is called")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                // should have a size of 3 if it were to match the range size
                std::vector<int> ratios {1, 2};

                REQUIRE_THROWS_AS(
                    aleatoric::Ratio(std::make_unique<aleatoric::DiscreteGenerator>(),
                                  std::make_unique<aleatoric::Range>(10, 12),
                                  ratios),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    aleatoric::Ratio(std::make_unique<aleatoric::DiscreteGenerator>(),
                                  std::make_unique<aleatoric::Range>(10, 12),
                                  ratios),
                    "The size of ratios collection must match the size of the "
                    "range");
            }
        }
    }

    GIVEN("The object is being constructed")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<aleatoric::Range>(10, 12);

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
                aleatoric::Ratio(std::move(generator), std::move(range), ratios);
            }
        }
    }

    GIVEN("The class is instantiated with a range and a ratios collection")
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

        auto range = std::make_unique<aleatoric::Range>(10, 12);

        // 3 numbers because the range is inclusive
        std::vector<int> ratios {1, 3, 5};

        aleatoric::Ratio instance(std::move(generator), std::move(range), ratios);

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

        WHEN("A reset is requested")
        {
            THEN("It resets the generator distribution")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }
}
