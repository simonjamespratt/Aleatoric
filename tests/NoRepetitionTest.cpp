#include "NoRepetition.hpp"

#include "DiscreteGeneratorMock.hpp"
#include "Range.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Numbers::NoRepetition")
{
    GIVEN("The class is instantiated")
    {
        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        auto range = std::make_unique<aleatoric::Range>(1, 3);
        auto rangePointer = range.get();

        WHEN("The object is constructed")
        {
            THEN("The generator distribution is set to the range size")
            {
                REQUIRE_CALL(*generatorPointer,
                             setDistributionVector(rangePointer->size, 1.0));
                aleatoric::NoRepetition(std::move(generator), std::move(range));
            }
        }
    }

    GIVEN("The class is instantiated")
    {
        int generatedNumber = 1;

        auto generator = std::make_unique<DiscreteGeneratorMock>();
        auto generatorPointer = generator.get();

        ALLOW_CALL(*generatorPointer, updateDistributionVector(1.0));
        ALLOW_CALL(*generatorPointer,
                   updateDistributionVector(generatedNumber, 0.0));
        ALLOW_CALL(*generatorPointer, setDistributionVector(ANY(int), 1.0));

        auto range = std::make_unique<aleatoric::Range>(1, 3);
        auto rangePointer = range.get();

        aleatoric::NoRepetition instance(std::move(generator), std::move(range));

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
                REQUIRE(returnedNumber ==
                        generatedNumber + rangePointer->offset);
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
