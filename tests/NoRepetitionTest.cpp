#include "NoRepetition.hpp"

#include "DiscreteGenerator.hpp"
#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::NoRepetition: default constructor")
{
    using namespace aleatoric;

    NoRepetition instance(std::make_unique<DiscreteGenerator>());

    std::vector<int> set(1000);
    for(auto &&i : set) {
        i = instance.getIntegerNumber();
    }

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
    }

    THEN("Set should be within range")
    {
        for(auto &&i : set) {
            REQUIRE((i == 0 || i == 1));
        }
    }

    THEN("Set should have no direct repetition")
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

SCENARIO("Numbers::NoRepetition")
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
                NoRepetition(std::move(generator), range);
            }
        }
    }

    GIVEN("The object is constructed")
    {
        int generatedNumber = 1;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, updateDistributionVector(1.0));
        ALLOW_CALL(*generatorPointer, updateDistributionVector(ANY(int), 0.0));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        Range range(1, 3);

        NoRepetition instance(std::move(generator), range);

        WHEN("A number is requested")
        {
            THEN("It should request a number from the generator")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                instance.getIntegerNumber();
            }

            THEN("It should return the generated number with the range offset "
                 "applied")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                auto returnedNumber = instance.getIntegerNumber();
                REQUIRE(returnedNumber == generatedNumber + range.offset);
            }

            THEN("It should ensure that the generated number cannot be "
                 "selected at the next time of asking whilst ensuring all "
                 "others have "
                 "equal probability of being selected")
            {
                REQUIRE_CALL(*generatorPointer, getNumber())
                    .RETURN(generatedNumber);
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                REQUIRE_CALL(*generatorPointer,
                             updateDistributionVector(generatedNumber, 0.0));
                instance.getIntegerNumber();
            }
        }

        WHEN("A reset is requested")
        {
            THEN("The generator should be set to uniform distribution - equal "
                 "probability")
            {
                REQUIRE_CALL(*generatorPointer, updateDistributionVector(1.0));
                instance.reset();
            }
        }
    }
}

SCENARIO("Numbers::NoRepetition: params")
{
    using namespace aleatoric;

    auto generator = std::make_unique<DiscreteGenerator>();
    auto generatorPointer = generator.get();
    NoRepetition instance(std::move(generator), Range(1, 10));

    WHEN("get params")
    {
        THEN("should match state of the object")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 10);
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocolParameters::Protocols::ActiveProtocol::
                        noRepetition);
        }
    }

    WHEN("set params")
    {
        Range newRange(20, 30);
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::NoRepetition()));
        instance.setParams(newParams);

        THEN("object state should be updated")
        {
            auto params = instance.getParams();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == newRange.start);
            REQUIRE(returnedRange.end == newRange.end);
        }
    }

    WHEN("set params: range")
    {
        GIVEN("No numbers have been returned yet")
        {
            Range newRange(11, 13);
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::NoRepetition()));
            instance.setParams(newParams);

            THEN("Generator distribution should be equal prob for new range")
            {
                REQUIRE(generatorPointer->getDistributionVector() ==
                        std::vector<double> {1, 1, 1});
            }
        }

        GIVEN("There is a last returned number")
        {
            auto lastNumber = instance.getIntegerNumber();

            WHEN("That is outside the new range")
            {
                Range newRange(lastNumber + 1, lastNumber + 4);
                NumberProtocolParameters newParams(
                    newRange,
                    NumberProtocolParameters::Protocols(
                        NumberProtocolParameters::NoRepetition()));
                instance.setParams(newParams);
                THEN(
                    "Generator distribution should be equal prob for new range")
                {
                    REQUIRE(generatorPointer->getDistributionVector() ==
                            std::vector<double> {1, 1, 1, 1});
                }
            }

            WHEN("That is within the new range")
            {
                Range newRange(lastNumber - 1, lastNumber + 1);
                NumberProtocolParameters newParams(
                    newRange,
                    NumberProtocolParameters::Protocols(
                        NumberProtocolParameters::NoRepetition()));
                instance.setParams(newParams);

                THEN("That number should be disallowed on the next selection")
                {
                    auto nextNumber = instance.getIntegerNumber();
                    REQUIRE(nextNumber != lastNumber);
                }
            }
        }
    }
}
