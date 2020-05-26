#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "Cycle.hpp"
#include "GranularWalk.hpp"
#include "GroupedRepetition.hpp"
#include "NoRepetition.hpp"
#include "NumberProtocolFactory.hpp"
#include "Periodic.hpp"
#include "Precision.hpp"
#include "Range.hpp"
#include "Ratio.hpp"
#include "Serial.hpp"
#include "Subset.hpp"
#include "Walk.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Numbers")
{
    int rangeStart = 0;
    int rangeEnd = 9;

    aleatoric::NumberProtocolFactory factory;

    GIVEN("An instance of the AdjacentSteps protocol is requested")
    {
        WHEN("Requested without an initial selection")
        {
            auto createdInstance =
                factory.createAdjacentSteps(rangeStart, rangeEnd);

            THEN("It returns an instance of the AdjacentSteps class")
            {
                auto castInstance = dynamic_cast<aleatoric::AdjacentSteps *>(
                    createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            auto createdInstance =
                factory.createAdjacentSteps(rangeStart, rangeEnd, 5);
            THEN("It returns an instance of the AdjacentSteps class")
            {
                auto castInstance = dynamic_cast<aleatoric::AdjacentSteps *>(
                    createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Basic protocol is requested")
    {
        WHEN("Requested")
        {
            auto createdInstance = factory.createBasic(rangeStart, rangeEnd);

            THEN("It returns an instance of the Basic class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Basic *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Cycle protocol is requested")
    {
        WHEN("Requested without an initial selection")
        {
            AND_WHEN("In the default state")
            {
                auto createdInstance =
                    factory.createCycle(rangeStart, rangeEnd);

                THEN("It returns an instance of the Cycle class")
                {
                    auto castInstance =
                        dynamic_cast<aleatoric::Cycle *>(createdInstance.get());

                    REQUIRE(castInstance != nullptr);
                }
            }

            AND_WHEN("Optional parameters are provided")
            {
                auto createdInstance =
                    factory.createCycle(rangeStart, rangeEnd, true, true);

                THEN("It returns an instance of the Cycle class")
                {
                    auto castInstance =
                        dynamic_cast<aleatoric::Cycle *>(createdInstance.get());

                    REQUIRE(castInstance != nullptr);
                }
            }
        }

        WHEN("Requested with an initial selection")
        {
            AND_WHEN("In the default state")
            {
                auto createdInstance =
                    factory.createCycle(rangeStart, rangeEnd, 5);

                THEN("It returns an instance of the Cycle class")
                {
                    auto castInstance =
                        dynamic_cast<aleatoric::Cycle *>(createdInstance.get());

                    REQUIRE(castInstance != nullptr);
                }
            }

            AND_WHEN("Optional parameters are provided")
            {
                auto createdInstance =
                    factory.createCycle(rangeStart, rangeEnd, 5, true, true);

                THEN("It returns an instance of the Cycle class")
                {
                    auto castInstance =
                        dynamic_cast<aleatoric::Cycle *>(createdInstance.get());

                    REQUIRE(castInstance != nullptr);
                }
            }
        }
    }

    GIVEN("An instance of the GroupedRepetition protocol is requested")
    {
        WHEN("Requested")
        {
            auto createdInstance =
                factory.createGroupedRepetition(rangeStart,
                                                rangeEnd,
                                                std::vector<int> {1, 2, 3});

            THEN("It returns an instance of the GroupedRepetition class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::GroupedRepetition *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the NoRepetition protocol is requested")
    {
        WHEN("Requested")
        {
            auto createdInstance =
                factory.createNoRepetition(rangeStart, rangeEnd);

            THEN("It returns an instance of the NoRepetition class")
            {
                auto castInstance = dynamic_cast<aleatoric::NoRepetition *>(
                    createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Periodic protocol is requested")
    {
        WHEN("Requested without an initial selection")
        {
            auto createdInstance =
                factory.createPeriodic(rangeStart, rangeEnd, 0.5);

            THEN("It returns an instance of the Periodic class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Periodic *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            auto createdInstance =
                factory.createPeriodic(rangeStart, rangeEnd, 0.5, 5);

            THEN("It returns an instance of the Periodic class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Periodic *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Precision protocol is requested")
    {
        // TODO: DOUBLE-SUMMING-PRECISION: Have to use a different range for
        // this set of tests due to an issue with the argument checking in
        // Precision for summing the values in the distribution. It should be
        // set back to a range of (0, 9) when this is fixed.
        aleatoric::Range referenceRange(1, 4);
        // NB: Precision requires that the distribution match the range size and
        // that the sum of the values for each number in the range equal 1.0.
        // There is an argument for testing the sad path here (invalid argument
        // exception) but we don't do that for any of the other protocols in
        // these tests.
        std::vector<double> distribution(referenceRange.size);
        for(auto &&i : distribution) {
            i = 0.25;
        }

        WHEN("Requested without an initial selection")
        {
            auto createdInstance = factory.createPrecision(referenceRange.start,
                                                           referenceRange.end,
                                                           distribution);

            THEN("It returns an instance of the Precision class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Precision *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            auto createdInstance = factory.createPrecision(referenceRange.start,
                                                           referenceRange.end,
                                                           distribution,
                                                           2);

            THEN("It returns an instance of the Precision class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Precision *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Ratio protocol is requested")
    {
        WHEN("Requested")
        {
            auto createdInstance =
                factory.createRatio(1, 3, std::vector<int> {1, 2, 3});

            THEN("It returns an instance of the Ratio class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Ratio *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Serial protocol is requested")
    {
        WHEN("Requested")
        {
            auto createdInstance = factory.createSerial(rangeStart, rangeEnd);

            THEN("It returns an instance of the Serial class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Serial *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Subset protocol is requested")
    {
        WHEN("Requested")
        {
            auto createdInstance =
                factory.createSubset(rangeStart, rangeEnd, 1, 2);

            THEN("It returns an instance of the Subset class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Subset *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the Walk protocol is requested")
    {
        WHEN("Requested without an initial selection")
        {
            auto createdInstance = factory.createWalk(rangeStart, rangeEnd, 1);

            THEN("It returns an instance of the Walk class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Walk *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            auto createdInstance =
                factory.createWalk(rangeStart, rangeEnd, 1, 5);

            THEN("It returns an instance of the Walk class")
            {
                auto castInstance =
                    dynamic_cast<aleatoric::Walk *>(createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }

    GIVEN("An instance of the GranularWalk protocol is requested")
    {
        WHEN("Requested without an initial selection")
        {
            auto createdInstance =
                factory.createGranularWalk(rangeStart, rangeEnd, 0.5);

            THEN("It returns an instance of the GranularWalk class")
            {
                auto castInstance = dynamic_cast<aleatoric::GranularWalk *>(
                    createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            auto createdInstance =
                factory.createGranularWalk(rangeStart, rangeEnd, 0.5, 5.0);

            THEN("It returns an instance of the GranularWalk class")
            {
                auto castInstance = dynamic_cast<aleatoric::GranularWalk *>(
                    createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }
    }
}
