#include "Cycle.hpp"

#include "Range.hpp"

#include <catch2/catch.hpp>
#include <memory>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Cycle")
{
    GIVEN("The object is constructed, with default settings: unidirectional, "
          "forward direction")
    {
        aleatoric::Range range(1, 3);
        aleatoric::Cycle instance(range);

        WHEN("A set of numbers matching the size of the range is requested")
        {
            std::vector<int> set(range.size);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("A collection representing the full range should have been "
                 "obtained")
            {
                std::vector<int> expectedSet {1, 2, 3};

                REQUIRE(set == expectedSet);
            }
        }

        WHEN("A set of numbers matching twice the size of the range is "
             "requested")
        {
            std::vector<int> set(range.size * 2);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
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
            // get the first number
            instance.getIntegerNumber();

            instance.reset();

            std::vector<int> set(range.size);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("It should behave as it would upon instantiation")
            {
                // next number without reset should be 2
                std::vector<int> expectedSet {1, 2, 3};

                REQUIRE(set == expectedSet);
            }
        }

        WHEN("The range is changed")
        {
            THEN("The returned range should match the new range")
            {
                instance.setRange(aleatoric::Range(5, 10));
                auto returnedRange = instance.getRange();
                REQUIRE(returnedRange.start == 5);
                REQUIRE(returnedRange.end == 10);
            }

            AND_WHEN("No numbers have been selected yet")
            {
                // demonstrated well when new range includes old range
                aleatoric::Range newRange(range.start - 1,
                                          range.end + 1); // 0 - 4
                instance.setRange(newRange);

                THEN("The cycle jumps to the start of the new range")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set ==
                            std::vector<int> {0, 1, 2, 3, 4, 0, 1, 2, 3, 4});
                }
            }

            AND_WHEN("The last selected number is mid-positioned in the "
                     "new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber - 1,
                                          lastNumber + 1); // 0 - 2
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 0, 1, 2, 0, 1});
                }
            }

            AND_WHEN("The last selected number is at the end of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber - 2,
                                          lastNumber); // -1 - 1
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {-1, 0, 1, -1, 0, 1});
                }
            }

            AND_WHEN(
                "The last selected number is at the start of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber,
                                          lastNumber + 3); // 1 - 4
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 3, 4, 1, 2, 3, 4, 1});
                }
            }

            AND_WHEN("The last selected number is above the end of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber - 3,
                                          lastNumber - 1); // -2 - 0
                instance.setRange(newRange);

                THEN("The cycle jumps to the start of the new range")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {-2, -1, 0, -2, -1, 0});
                }
            }

            AND_WHEN("The last selected number is below the start of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber + 1,
                                          lastNumber + 3); // 2 - 4
                instance.setRange(newRange);

                THEN("The cycle jumps to the start of the new range")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 3, 4, 2, 3, 4});
                }
            }
        }
    }

    GIVEN("The object is constructed: unidirectional, reverse direction")
    {
        aleatoric::Range range(1, 3);
        aleatoric::Cycle instance(range, false, true);

        WHEN("A set of numbers matching the size of the range is requested")
        {
            std::vector<int> set(range.size);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("A collection representing the full range should have been "
                 "obtained")
            {
                std::vector<int> expectedSet {3, 2, 1};

                REQUIRE(set == expectedSet);
            }
        }

        WHEN("A set of numbers matching twice the size of the range is "
             "requested")
        {
            std::vector<int> set(range.size * 2);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
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
            // get the first number
            instance.getIntegerNumber();

            instance.reset();

            std::vector<int> set(range.size);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("It should behave as it would upon instantiation")
            {
                // next number without reset should be 2
                std::vector<int> expectedSet {3, 2, 1};

                REQUIRE(set == expectedSet);
            }
        }

        WHEN("The range is changed")
        {
            AND_WHEN("No numbers have been selected yet")
            {
                // demonstrated well when new range includes old range
                aleatoric::Range newRange(range.start - 1,
                                          range.end + 1); // 0 - 4
                instance.setRange(newRange);

                THEN("The cycle jumps to the end of the new range")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set ==
                            std::vector<int> {4, 3, 2, 1, 0, 4, 3, 2, 1, 0});
                }
            }

            AND_WHEN("The last selected number is mid-positioned in the "
                     "new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber - 1,
                                          lastNumber + 1); // 2 - 4
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 4, 3, 2, 4, 3});
                }
            }

            AND_WHEN("The last selected number is at the end of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber - 2,
                                          lastNumber); // 1 - 3
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 1, 3, 2, 1, 3});
                }
            }

            AND_WHEN(
                "The last selected number is at the start of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber,
                                          lastNumber + 2); // 3 - 5
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {5, 4, 3, 5, 4, 3});
                }
            }

            AND_WHEN("The last selected number is above the end of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber - 3,
                                          lastNumber - 1); // 0 - 2
                instance.setRange(newRange);

                THEN("The cycle jumps to the end of the new range")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 1, 0, 2, 1, 0});
                }
            }

            AND_WHEN("The last selected number is below the start of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber + 1,
                                          lastNumber + 3); // 4 - 6
                instance.setRange(newRange);

                THEN("The cycle jumps to the end of the new range")
                {
                    std::vector<int> set(newRange.size * 2);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {6, 5, 4, 6, 5, 4});
                }
            }
        }
    }

    GIVEN("The object is constructed: bidirectional, forward direction")
    {
        aleatoric::Range range(1, 3);
        aleatoric::Cycle instance(range, true);

        WHEN("A set of numbers representing a full forward then reverse "
             "traversal of the range has been requested")
        {
            // minus one because there should be no repetition of the
            // numbers at the range ends
            std::vector<int> set(range.size * 2 - 1);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
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
            // minus three because there should be no repetition of the
            // numbers at the range ends
            std::vector<int> set(range.size * 4 - 3);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
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
            // get the first number
            instance.getIntegerNumber();

            instance.reset();

            // minus one because there should be no repetition of the
            // numbers at the range ends
            std::vector<int> set(range.size * 2 - 1);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("It should behave as it would upon instantiation")
            {
                // next number without reset should be 2
                std::vector<int> expectedSet {1, 2, 3, 2, 1};

                REQUIRE(set == expectedSet);
            }
        }

        WHEN("The range is changed")
        {
            AND_WHEN("No numbers have been selected yet")
            {
                // demonstrated well when new range includes old range
                aleatoric::Range newRange(range.start - 1,
                                          range.end + 1); // 0 - 4
                instance.setRange(newRange);

                THEN("The cycle jumps to the start of the new range")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set ==
                            std::vector<int> {0, 1, 2, 3, 4, 3, 2, 1, 0});
                }
            }

            AND_WHEN("The last selected number is mid-positioned in the "
                     "new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber - 1,
                                          lastNumber + 1); // 0 - 2
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 1, 0, 1, 2});
                }
            }

            AND_WHEN("The last selected number is at the end of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber - 2,
                                          lastNumber); // -1 - 1
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {0, -1, 0, 1, 0});
                }
            }

            AND_WHEN("The last selected number is at the end of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber,
                                          lastNumber + 2); // 1 - 3
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 3, 2, 1, 2});
                }
            }

            AND_WHEN("The last selected number is above the end of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber - 3,
                                          lastNumber - 1); // -2 - -0
                instance.setRange(newRange);

                THEN("The cycle jumps to the start of the new range")
                {
                    // in other words, whichever direction of travel it is
                    // currently going in
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {-2, -1, 0, -1, -2});
                }
            }

            AND_WHEN("The last selected number is below the start of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 1
                aleatoric::Range newRange(lastNumber + 1,
                                          lastNumber + 3); // 2 - 4
                instance.setRange(newRange);

                THEN("The cycle jumps to the start the new range")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 3, 4, 3, 2});
                }
            }
        }
    }

    GIVEN("The object is constructed: bidirectional, reverse direction")
    {
        aleatoric::Range range(1, 3);
        aleatoric::Cycle instance(range, true, true);

        WHEN("A set of numbers representing a full forward then reverse "
             "traversal of the range has been requested")
        {
            // minus one because there should be no repetition of the
            // numbers at the range ends
            std::vector<int> set(range.size * 2 - 1);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
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
            // minus three because there should be no repetition of the
            // numbers at the range ends
            std::vector<int> set(range.size * 4 - 3);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
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
            // get the first number
            instance.getIntegerNumber();

            instance.reset();

            // minus one because there should be no repetition of the
            // numbers at the range ends
            std::vector<int> set(range.size * 2 - 1);

            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            THEN("It should behave as it would upon instantiation")
            {
                // next number without reset should be 2
                std::vector<int> expectedSet {3, 2, 1, 2, 3};

                REQUIRE(set == expectedSet);
            }
        }

        WHEN("The range is changed")
        {
            AND_WHEN("No numbers have been selected yet")
            {
                // demonstrated well when new range includes old range
                aleatoric::Range newRange(range.start - 1,
                                          range.end + 1); // 0 - 4
                instance.setRange(newRange);

                THEN("The cycle jumps to the end of the new range")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set ==
                            std::vector<int> {4, 3, 2, 1, 0, 1, 2, 3, 4});
                }
            }

            AND_WHEN("The last selected number is mid-positioned in the "
                     "new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber - 1,
                                          lastNumber + 1); // 2 - 4
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 3, 4, 3, 2});
                }
            }

            AND_WHEN("The last selected number is at the end of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber - 2,
                                          lastNumber); // 1 - 3
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 1, 2, 3, 2});
                }
            }

            AND_WHEN(
                "The last selected number is at the start of the new range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber,
                                          lastNumber + 2); // 3 - 5
                instance.setRange(newRange);

                THEN("The cycle continues as expected")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {4, 5, 4, 3, 4});
                }
            }

            AND_WHEN("The last selected number is above the end of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber - 3,
                                          lastNumber - 1); // 0 - 2
                instance.setRange(newRange);

                THEN("The cycle jumps to the end of the new range")
                {
                    // in other words, whichever direction of travel it is
                    // currently going in
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {2, 1, 0, 1, 2});
                }
            }

            AND_WHEN("The last selected number is below the start of the new "
                     "range")
            {
                // Old range 1, 3
                auto lastNumber = instance.getIntegerNumber(); // 3
                aleatoric::Range newRange(lastNumber + 1,
                                          lastNumber + 3); // 4 - 6
                instance.setRange(newRange);

                THEN("The cycle jumps to the start the new range")
                {
                    std::vector<int> set(newRange.size * 2 - 1);
                    for(auto &&i : set) {
                        i = instance.getIntegerNumber();
                    }

                    REQUIRE(set == std::vector<int> {6, 5, 4, 5, 6});
                }
            }
        }
    }
}
