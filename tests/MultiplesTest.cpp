#include "Multiples.hpp"

#include "Range.hpp"
#include "UniformGenerator.hpp"
#include "UniformGeneratorMock.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("TimeDomain::Multiples")
{
    GIVEN(
        "The class is instantiated with a range and an invalid base increment")
    {
        WHEN("The value supplied is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidBaseIncrement = 0;

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(invalidBaseIncrement,
                                                  actlib::Numbers::Range(2, 4)),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(invalidBaseIncrement,
                                                  actlib::Numbers::Range(2, 4)),
                    "The base increment supplied must be equal to, or greater "
                    "than, 1");
            }
        }
    }

    GIVEN("The class is instantiated with an invalid range")
    {
        int baseIncrement = 100;

        WHEN("The range start is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(baseIncrement,
                                                  actlib::Numbers::Range(0, 4)),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(baseIncrement,
                                                  actlib::Numbers::Range(0, 4)),
                    "The range object supplied must have a start value equal "
                    "to, or "
                    "greater than, 1");
            }
        }
    }

    GIVEN("The class is instantiated with a range")
    {
        int baseIncrement = 100;
        actlib::Numbers::Range range(2, 4);
        actlib::TimeDomain::Multiples instance(baseIncrement, range);

        WHEN("The size of the duration collection is requested")
        {
            THEN("The returned number should match the size of the range "
                 "provided at construction")
            {
                auto size = instance.getCollectionSize();
                REQUIRE(size == range.size);
            }
        }

        WHEN("Each duration is requested")
        {
            THEN("It should equal that number in the range multiplied by the "
                 "base increment")
            {
                for(int i = 0; i < range.size; i++) {
                    REQUIRE(instance.getDuration(i) ==
                            (i + range.offset) * baseIncrement);
                }
            }
        }
    }

    GIVEN("The class is instantiated with a range and an invalid deviation "
          "factor")
    {
        WHEN("The value provided is less than 0.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int baseIncrement = 100;
                double invalidDeviation = -0.01;
                actlib::Numbers::Range range(2, 4);

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        range,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        range,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }

        WHEN("The value provided is more than 1.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int baseIncrement = 100;
                double invalidDeviation = 1.01;
                actlib::Numbers::Range range(2, 4);

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        range,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        range,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }
    }

    GIVEN("The class is instantiated with a range and a deviationFactor")
    {
        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));
        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(1);

        int baseIncrement = 100;
        double deviationFactor = 0.5;
        actlib::Numbers::Range range(2, 4);

        actlib::TimeDomain::Multiples instance(baseIncrement,
                                               range,
                                               deviationFactor,
                                               std::move(generator));

        WHEN("The size of the duration collection is requested")
        {
            THEN("The returned number should match the size of the range "
                 "provided at construction")
            {
                auto size = instance.getCollectionSize();
                REQUIRE(size == range.size);
            }
        }

        WHEN("Each duration is requested")
        {
            THEN("The generator should be set with a range around the duration "
                 "that matches the deviation factor supplied")
            {
                // for a range of 2-4 with a base increment of 100 and a
                // deviation factor of 0.5, the generator ranges set should be:

                // 200 = 100 - 300
                // 300 = 150 - 450
                // 400 = 200 - 600

                REQUIRE_CALL(*generatorPointer, setDistribution(100, 300));
                REQUIRE_CALL(*generatorPointer, setDistribution(150, 450));
                REQUIRE_CALL(*generatorPointer, setDistribution(200, 600));
                instance.getDuration(0);
                instance.getDuration(1);
                instance.getDuration(2);
            }

            THEN("The generator should be called to select a number which "
                 "should be returned")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(123);
                auto returnedNumber = instance.getDuration(0);
                REQUIRE(returnedNumber == 123);
            }

            AND_WHEN("The deviation factor causes rounding to occur")
            {
                // These two in combination will create deviations that require
                // rounding
                int roundingBase = 33;
                double roundingDevF = 0.3;

                auto roundingGen = std::make_unique<UniformGeneratorMock>();
                auto roundingGenPointer = roundingGen.get();
                ALLOW_CALL(*roundingGenPointer, getNumber()).RETURN(1);

                actlib::Numbers::Range roundingRange(1, 3);

                actlib::TimeDomain::Multiples roundingInstance(
                    roundingBase,
                    roundingRange,
                    roundingDevF,
                    std::move(roundingGen));

                THEN("The generator should be set with a range around the "
                     "duration where the numbers have been correctly rounded")
                {
                    // for a range of 1-3 with a base increment of 33 and a
                    // deviation factor of 0.3, the generator ranges set should
                    // be:

                    // 33 = (33 - 9.9 = 23.1 ~ 23) -> (33 + 9.9 = 42.9 ~ 43)
                    // 66 = (66 - 19.8 = 46.2 ~ 46) -> (66 + 19.8 = 85.8 ~ 86)
                    // 99 = (99 - 29.7 = 69.3 ~ 69) -> (99 + 29.7 = 128.7 ~ 129)

                    REQUIRE_CALL(*roundingGenPointer, setDistribution(23, 43));
                    REQUIRE_CALL(*roundingGenPointer, setDistribution(46, 86));
                    REQUIRE_CALL(*roundingGenPointer, setDistribution(69, 129));
                    roundingInstance.getDuration(0);
                    roundingInstance.getDuration(1);
                    roundingInstance.getDuration(2);
                }
            }
        }
    }

    GIVEN("The class is instantiated with a multipliers collection and an "
          "invalid base increment")
    {
        WHEN("The value supplied is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int invalidBaseIncrement = 0;

                REQUIRE_THROWS_AS(actlib::TimeDomain::Multiples(
                                      invalidBaseIncrement,
                                      std::vector<int> {3, 5, 7, 9}),
                                  std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(
                        invalidBaseIncrement,
                        std::vector<int> {3, 5, 7, 9}),
                    "The base increment supplied must be equal to, or greater "
                    "than, 1");
            }
        }
    }

    GIVEN("The class is instantiated with a multipliers collection where one "
          "of the values is invalid")
    {
        WHEN("A value in the collection is less than 1")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int baseIncrement = 100;
                std::vector<int> multipliers {0, 3, 5, 7};

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(baseIncrement, multipliers),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(baseIncrement, multipliers),
                    "The collection passed for the argument multipliers "
                    "contains an invalid value. Values must be equal to, or "
                    "greater than, 1");
            }
        }
    }

    GIVEN("The class is instantiated with a multipliers collection")
    {
        int baseIncrement = 100;
        std::vector<int> multipliers {3, 5, 7, 9};
        actlib::TimeDomain::Multiples instance(baseIncrement, multipliers);

        WHEN("The size of the duration collection is requested")
        {
            THEN("The returned number should match the size of the source "
                 "provided at construction")
            {
                auto size = instance.getCollectionSize();
                REQUIRE(size == multipliers.size());
            }
        }

        WHEN("Each duration is requested")
        {
            THEN("It should match the value at each index of the source "
                 "given "
                 "at construction when multipled by the base increment")
            {
                for(int i = 0; i < multipliers.size(); i++) {
                    REQUIRE(instance.getDuration(i) ==
                            multipliers[i] * baseIncrement);
                }
            }
        }
    }

    GIVEN("The class is instantiated with a multipliers collection and an "
          "invalid deviation factor")
    {
        WHEN("The value provided is less than 0.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int baseIncrement = 100;
                double invalidDeviation = -0.01;
                std::vector<int> multipliers {3, 5, 7, 9};

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        multipliers,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        multipliers,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }

        WHEN("The value provided is more than 1.0")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int baseIncrement = 100;
                double invalidDeviation = 1.01;
                std::vector<int> multipliers {3, 5, 7, 9};

                REQUIRE_THROWS_AS(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        multipliers,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::TimeDomain::Multiples(
                        baseIncrement,
                        multipliers,
                        invalidDeviation,
                        std::make_unique<actlib::Numbers::UniformGenerator>()),
                    "The value passed as argument for deviationFactor must be "
                    "within the range of 0.0 to 1.0");
            }
        }
    }

    GIVEN("The class is instantiated with a multipliers collection and a "
          "deviation factor")
    {
        auto generator = std::make_unique<UniformGeneratorMock>();
        auto generatorPointer = generator.get();
        ALLOW_CALL(*generatorPointer, setDistribution(ANY(int), ANY(int)));
        ALLOW_CALL(*generatorPointer, getNumber()).RETURN(1);

        int baseIncrement = 100;
        double deviationFactor = 0.5;
        std::vector<int> multipliers {3, 5, 7, 9};

        actlib::TimeDomain::Multiples instance(baseIncrement,
                                               multipliers,
                                               deviationFactor,
                                               std::move(generator));

        WHEN("The size of the duration collection is requested")
        {
            THEN("The returned number should match the size of the source "
                 "provided at construction")
            {
                auto size = instance.getCollectionSize();
                REQUIRE(size == multipliers.size());
            }
        }

        WHEN("Each duration is requested")
        {
            THEN("The generator should be set with a range around the duration "
                 "that matches the deviation factor supplied")
            {
                // for the given multiplier, with a base increment of 100 and a
                // deviation factor of 0.5, the generator ranges set should be:

                // 300 = 150 - 450
                // 500 = 250 - 750
                // 700 = 350 - 1050
                // 900 = 450 - 1350

                REQUIRE_CALL(*generatorPointer, setDistribution(150, 450));
                REQUIRE_CALL(*generatorPointer, setDistribution(250, 750));
                REQUIRE_CALL(*generatorPointer, setDistribution(350, 1050));
                REQUIRE_CALL(*generatorPointer, setDistribution(450, 1350));
                instance.getDuration(0);
                instance.getDuration(1);
                instance.getDuration(2);
                instance.getDuration(3);
            }

            THEN("The generator should be called to select a number which "
                 "should be returned")
            {
                REQUIRE_CALL(*generatorPointer, getNumber()).RETURN(123);
                auto returnedNumber = instance.getDuration(0);
                REQUIRE(returnedNumber == 123);
            }
        }
    }
}
