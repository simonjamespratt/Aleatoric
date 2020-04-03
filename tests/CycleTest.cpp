#include "Cycle.hpp"

#include "Range.hpp"

#include <catch2/catch.hpp>
#include <memory>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Cycle")
{
    GIVEN("The class is instantiated with default settings: unidirectional, "
          "forward direction")
    {
        AND_GIVEN("It was constructed without an initial selection")
        {
            WHEN("A set of numbers matching the size of the range is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range));

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN(
                    "A collection representing the full range should have been "
                    "obtained")
                {
                    std::vector<int> expectedSet {1, 2, 3};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A set of numbers matching twice the size of the range is "
                 "requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range));

                std::vector<int> set(rangePointer->size * 2);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the range twice, with no "
                     "descending aspect should have been obtained")
                {
                    std::vector<int> expectedSet {1, 2, 3, 1, 2, 3};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range));

                // get the first number
                instance.getNumber();

                instance.reset();

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 2
                    std::vector<int> expectedSet {1, 2, 3};

                    REQUIRE(set == expectedSet);
                }
            }
        }

        AND_GIVEN("It was constructed with an initial selection")
        {
            WHEN("A set of numbers matching the size of the range is requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection);

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN(
                    "A collection representing the full range should have been "
                    "obtained, starting from the point in the range described "
                    "by the initial selection")
                {
                    std::vector<int> expectedSet {2, 3, 1};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection);

                // get the first number
                instance.getNumber();

                instance.reset();

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 3
                    std::vector<int> expectedSet {2, 3, 1};

                    REQUIRE(set == expectedSet);
                }
            }
        }
    }

    GIVEN("The class is instantiated with settings: unidirectional, "
          "reverse direction")
    {
        AND_GIVEN("It was constructed without an initial selection")
        {
            WHEN("A set of numbers matching the size of the range is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       false,
                                                       true);

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN(
                    "A collection representing the full range should have been "
                    "obtained")
                {
                    std::vector<int> expectedSet {3, 2, 1};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A set of numbers matching twice the size of the range is "
                 "requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       false,
                                                       true);

                std::vector<int> set(rangePointer->size * 2);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the range twice, with no "
                     "descending aspect should hve been obtained")
                {
                    std::vector<int> expectedSet {3, 2, 1, 3, 2, 1};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       false,
                                                       true);

                // get the first number
                instance.getNumber();

                instance.reset();

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 2
                    std::vector<int> expectedSet {3, 2, 1};

                    REQUIRE(set == expectedSet);
                }
            }
        }

        AND_GIVEN("It was constructed with an initial selection")
        {
            WHEN("A set of numbers matching the size of the range is requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection,
                                                       false,
                                                       true);

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the full range in reverse "
                     "should have been obtained, starting from the point in "
                     "the range described by the initial selection")
                {
                    std::vector<int> expectedSet {2, 1, 3};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection,
                                                       false,
                                                       true);

                // get the first number
                instance.getNumber();

                instance.reset();

                std::vector<int> set(rangePointer->size);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 1
                    std::vector<int> expectedSet {2, 1, 3};

                    REQUIRE(set == expectedSet);
                }
            }
        }
    }

    GIVEN("The class is instantiated with settings: bidirectional, "
          "forward direction")
    {
        AND_GIVEN("It was constructed without an initial selection")
        {
            WHEN("A set of numbers representing a full forward then reverse "
                 "traversal of the range has been requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range), true);

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the described traversal should "
                     "have been obtained")
                {
                    std::vector<int> expectedSet {1, 2, 3, 2, 1};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("Two cycles of the range in bidirectional form is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range), true);

                // minus three because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 4 - 3);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the described traversal should "
                     "have been obtained")
                {
                    std::vector<int> expectedSet {1, 2, 3, 2, 1, 2, 3, 2, 1};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range), true);

                // get the first number
                instance.getNumber();

                instance.reset();

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 2
                    std::vector<int> expectedSet {1, 2, 3, 2, 1};

                    REQUIRE(set == expectedSet);
                }
            }
        }

        AND_GIVEN("It was constructed with an initial selection")
        {
            WHEN("A set of numbers representing a full forward then reverse "
                 "traversal of the range has been requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection,
                                                       true);

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the full range should have "
                     "been obtained, starting from the point in the range "
                     "described by the initial selection")
                {
                    std::vector<int> expectedSet {2, 3, 2, 1, 2};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection,
                                                       true);

                // get the first number
                instance.getNumber();

                instance.reset();

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 3
                    std::vector<int> expectedSet {2, 3, 2, 1, 2};

                    REQUIRE(set == expectedSet);
                }
            }
        }
    }

    GIVEN("The class is instantiated with settings: bidirectional, "
          "reverse direction")
    {
        AND_GIVEN("It was constructed without an initial selection")
        {
            WHEN("A set of numbers representing a full forward then reverse "
                 "traversal of the range has been requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       true,
                                                       true);

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the described traversal should "
                     "have been obtained")
                {
                    std::vector<int> expectedSet {3, 2, 1, 2, 3};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("Two cycles of the range in bidirectional form is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       true,
                                                       true);

                // minus three because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 4 - 3);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the described traversal should "
                     "have been obtained")
                {
                    std::vector<int> expectedSet {3, 2, 1, 2, 3, 2, 1, 2, 3};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       true,
                                                       true);

                // get the first number
                instance.getNumber();

                instance.reset();

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 2
                    std::vector<int> expectedSet {3, 2, 1, 2, 3};

                    REQUIRE(set == expectedSet);
                }
            }
        }

        AND_GIVEN("It was constructed with an initial selection")
        {
            WHEN("A set of numbers representing a full forward then reverse "
                 "traversal of the range has been requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection,
                                                       true,
                                                       true);

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("A collection representing the full range should have "
                     "been obtained, starting from the point in the range "
                     "described by the initial selection")
                {
                    std::vector<int> expectedSet {2, 1, 2, 3, 2};

                    REQUIRE(set == expectedSet);
                }
            }

            WHEN("A reset is requested")
            {
                int initialSelection = 2;

                auto range = std::make_unique<actlib::Numbers::Range>(1, 3);
                auto rangePointer = range.get();

                actlib::Numbers::Steps::Cycle instance(std::move(range),
                                                       initialSelection,
                                                       true,
                                                       true);

                // get the first number
                instance.getNumber();

                instance.reset();

                // minus one because there should be no repetition of the
                // numbers at the range ends
                std::vector<int> set(rangePointer->size * 2 - 1);

                for(auto &&i : set) {
                    i = instance.getNumber();
                }

                THEN("It should behave as it would upon instantiation")
                {
                    // next number without reset should be 1
                    std::vector<int> expectedSet {2, 1, 2, 3, 2};

                    REQUIRE(set == expectedSet);
                }
            }
        }
    }

    GIVEN("The class is instantiated with with an invalid initial selection "
          "value")
    {
        WHEN("The value provided is greater than the range end")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelectionOutOfRange = 4;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::Cycle(
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::Cycle(
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }

        WHEN("The value provided is less than the range start")
        {
            THEN("A standard invalid_argument exception is thrown")
            {
                int initialSelectionOutOfRange = 0;

                REQUIRE_THROWS_AS(
                    actlib::Numbers::Steps::Cycle(
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    std::invalid_argument);

                REQUIRE_THROWS_WITH(
                    actlib::Numbers::Steps::Cycle(
                        std::make_unique<actlib::Numbers::Range>(1, 3),
                        initialSelectionOutOfRange),
                    "The value passed as argument for initialSelection must be "
                    "within the range of 1 to 3");
            }
        }
    }
}
