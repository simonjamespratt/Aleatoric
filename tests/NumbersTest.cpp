#include "Numbers.hpp"

#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "GranularWalk.hpp"
#include "NoRepetition.hpp"
#include "Periodic.hpp"
#include "Serial.hpp"
#include "Walk.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Numbers")
{
    int rangeStart = 0;
    int rangeEnd = 9;

    actlib::Numbers::Numbers factory;

    GIVEN("An instance of the AdjacentSteps protocol is requested")
    {
        WHEN("Requested without an initial selection")
        {
            auto createdInstance =
                factory.createAdjacentSteps(rangeStart, rangeEnd);

            THEN("It returns an instance of the AdjacentSteps class")
            {
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Steps::AdjacentSteps *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            int initialSelection = 5;

            auto createdInstance =
                factory.createAdjacentSteps(rangeStart,
                                            rangeEnd,
                                            initialSelection);
            THEN("It returns an instance of the AdjacentSteps class")
            {
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Steps::AdjacentSteps *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }

            THEN(
                "It returns the initialSelection on first call to get a number")
            {
                auto number = createdInstance->getNumber();
                REQUIRE(number == initialSelection);
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
                    dynamic_cast<actlib::Numbers::Steps::Basic *>(
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
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Steps::NoRepetition *>(
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
                    dynamic_cast<actlib::Numbers::Steps::Periodic *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            int initialSelection = 5;

            auto createdInstance = factory.createPeriodic(rangeStart,
                                                          rangeEnd,
                                                          0.5,
                                                          initialSelection);

            THEN("It returns an instance of the Periodic class")
            {
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Steps::Periodic *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }

            THEN(
                "It returns the initialSelection on first call to get a number")
            {
                auto number = createdInstance->getNumber();
                REQUIRE(number == initialSelection);
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
                    dynamic_cast<actlib::Numbers::Steps::Serial *>(
                        createdInstance.get());

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
                    dynamic_cast<actlib::Numbers::Steps::Walk *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            int initialSelection = 5;

            auto createdInstance =
                factory.createWalk(rangeStart, rangeEnd, 1, initialSelection);

            THEN("It returns an instance of the Walk class")
            {
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Steps::Walk *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }

            THEN(
                "It returns the initialSelection on first call to get a number")
            {
                auto number = createdInstance->getNumber();
                REQUIRE(number == initialSelection);
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
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Granular::GranularWalk *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }
        }

        WHEN("Requested with an initial selection")
        {
            double initialSelection = 5.0;

            auto createdInstance = factory.createGranularWalk(rangeStart,
                                                              rangeEnd,
                                                              0.5,
                                                              initialSelection);

            THEN("It returns an instance of the GranularWalk class")
            {
                auto castInstance =
                    dynamic_cast<actlib::Numbers::Granular::GranularWalk *>(
                        createdInstance.get());

                REQUIRE(castInstance != nullptr);
            }

            THEN(
                "It returns the initialSelection on first call to get a number")
            {
                auto number = createdInstance->getNumber();
                REQUIRE(number == initialSelection);
            }
        }
    }
}
