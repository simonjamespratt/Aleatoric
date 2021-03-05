#include "Serial.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <array>
#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::Serial: default constructor")
{
    using namespace aleatoric;

    Serial instance(std::make_unique<DiscreteGenerator>());

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
    }

    THEN("Set of numbers should be as per basic serial process")
    {
        // basic serial process
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

SCENARIO("Numbers::Serial")
{
    using namespace aleatoric;

    GIVEN("Construction")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        Range range(1, 3);

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(range.size, 1.0));
                Serial(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int generatedNumber = 1;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(generatedNumber);
        ALLOW_CALL(*generatorPointer,
                   updateDistributionVector(generatedNumber, 0.0));

        // ensures that seriesIsComplete returns true
        ALLOW_CALL(*generatorPointer, getDistributionVector())
            .RETURN(std::vector<double> {1.0});

        Range range(1, 3);

        Serial instance(std::move(generator), range);

        WHEN("A number is requested")
        {
            THEN("It calls the generator to get a number")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                instance.getIntegerNumber();
            }

            THEN("It updates the generator to disallow the selected number "
                 "from being selected in future calls")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getIntegerNumber();
            }

            THEN("It returns the selected number, offset by the range.offset")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto number = instance.getIntegerNumber();
                REQUIRE(number == generatedNumber + range.offset);
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

SCENARIO("Numbers::Serial: params")
{
    using namespace aleatoric;

    Serial instance(std::make_unique<DiscreteGenerator>(), Range(1, 3));

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("should match state of the object")
        {
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 3);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocol::Type::serial);
        }
    }

    WHEN("Set params")
    {
        Range newRange(4, 9);
        NumberProtocolConfig newParams(newRange,
                                       NumberProtocolParams(SerialParams()));
        instance.setParams(newParams);

        THEN("The object state is updated")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }

        THEN("A set covering the range will have the expected result")
        {
            std::vector<int> set(newRange.size);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            std::vector<int> expectedValues {4, 5, 6, 7, 8, 9};
            auto expectedValuesPermutation =
                Catch::UnorderedEquals(expectedValues);

            REQUIRE_THAT(set, expectedValuesPermutation);
        }
    }
}
