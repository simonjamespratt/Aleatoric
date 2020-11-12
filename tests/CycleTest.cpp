#include "Cycle.hpp"

#include "Range.hpp"

#include <catch2/catch.hpp>
#include <memory>
#include <stdexcept> // std::invalid_argument

SCENARIO("Numbers::Cycle: default constructor: unidirectional forwards")
{
    using namespace aleatoric;

    Cycle instance;

    THEN("Params are set to defaults")
    {
        auto params = instance.getParams();
        auto range = params.getRange();
        auto cycleParams = params.protocols.getCycle();

        REQUIRE(range.start == 0);
        REQUIRE(range.end == 1);
        REQUIRE_FALSE(cycleParams.getBidirectional());
        REQUIRE_FALSE(cycleParams.getReverseDirection());
    }

    THEN("A pair of cycles should be as expected")
    {
        std::vector<int> expectedResult {0, 1, 0, 1};
        std::vector<int> set(expectedResult.size());
        for(auto &&i : set) {
            i = instance.getIntegerNumber();
        }

        REQUIRE_THAT(set, Catch::Equals(expectedResult));
    }
}

SCENARIO("Numbers::Cycle")
{
    using namespace aleatoric;

    GIVEN("The object is constructed, with default settings: unidirectional, "
          "forward direction")
    {
        Range range(1, 3);
        Cycle instance(range);

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
    }

    GIVEN("The object is constructed: unidirectional, reverse direction")
    {
        Range range(1, 3);
        Cycle instance(range, false, true);

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
    }

    GIVEN("The object is constructed: bidirectional, forward direction")
    {
        Range range(1, 3);
        Cycle instance(range, true);

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
    }

    GIVEN("The object is constructed: bidirectional, reverse direction")
    {
        Range range(1, 3);
        Cycle instance(range, true, true);

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
    }
}

SCENARIO("Numbers::Cycle: params")
{
    using namespace aleatoric;

    Cycle instance(Range(1, 3));

    WHEN("Get params")
    {
        auto params = instance.getParams();
        auto returnedRange = params.getRange();

        THEN("They should match the state of the object")
        {
            auto cycleParams = params.protocols.getCycle();
            REQUIRE(returnedRange.start == 1);
            REQUIRE(returnedRange.end == 3);
            REQUIRE_FALSE(cycleParams.getBidirectional());
            REQUIRE_FALSE(cycleParams.getReverseDirection());
            REQUIRE(params.protocols.getActiveProtocol() ==
                    NumberProtocolParameters::Protocols::ActiveProtocol::cycle);
        }
    }

    WHEN("Set params")
    {
        NumberProtocolParameters newParams(
            Range(10, 20),
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Cycle(true, true)));
        instance.setParams(newParams);

        THEN("params should be updated")
        {
            auto params = instance.getParams();
            auto cycleParams = params.protocols.getCycle();
            auto returnedRange = params.getRange();
            REQUIRE(returnedRange.start == 10);
            REQUIRE(returnedRange.end == 20);
            REQUIRE(cycleParams.getBidirectional());
            REQUIRE(cycleParams.getReverseDirection());
        }
    }
}

SCENARIO("Numbers::Cycle: Set params: no numbers returned yet")
{
    using namespace aleatoric;

    Range range(1, 3);

    // demonstrated well when new range includes old range
    Range newRange(range.start - 1, range.end + 1); // 0-4

    GIVEN("State set to unidirectional, forward direction")
    {
        Cycle instance(range, true,
                       true); // make it start with different state

        bool newBidirectional = false;
        bool newReverseDirection = false;
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Cycle(newBidirectional,
                                                newReverseDirection)));
        instance.setParams(newParams);

        THEN("A set of numbers should be as expected")
        {
            std::vector<int> set(newRange.size * 2);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            REQUIRE(set == std::vector<int> {0, 1, 2, 3, 4, 0, 1, 2, 3, 4});
        }
    }

    GIVEN("State set to unidirectional, reverse direction")
    {
        Cycle instance(range,
                       true,
                       false); // make it start with different state

        bool newBidirectional = false;
        bool newReverseDirection = true;
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Cycle(newBidirectional,
                                                newReverseDirection)));
        instance.setParams(newParams);

        THEN("A set of numbers should be as expected")
        {
            std::vector<int> set(newRange.size * 2);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            REQUIRE(set == std::vector<int> {4, 3, 2, 1, 0, 4, 3, 2, 1, 0});
        }
    }

    GIVEN("State set to bidirectional, forward direction")
    {
        Cycle instance(range,
                       false,
                       true); // make it start with different state

        bool newBidirectional = true;
        bool newReverseDirection = false;
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Cycle(newBidirectional,
                                                newReverseDirection)));
        instance.setParams(newParams);

        THEN("A set of numbers should be as expected")
        {
            std::vector<int> set(newRange.size * 2 - 1);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            REQUIRE(set == std::vector<int> {0, 1, 2, 3, 4, 3, 2, 1, 0});
        }
    }

    GIVEN("State set to bidirectional, reverse direction")
    {
        Cycle instance(range,
                       false,
                       false); // make it start with different state

        bool newBidirectional = true;
        bool newReverseDirection = true;
        NumberProtocolParameters newParams(
            newRange,
            NumberProtocolParameters::Protocols(
                NumberProtocolParameters::Cycle(newBidirectional,
                                                newReverseDirection)));
        instance.setParams(newParams);

        THEN("A set of numbers should be as expected")
        {
            std::vector<int> set(newRange.size * 2 - 1);
            for(auto &&i : set) {
                i = instance.getIntegerNumber();
            }

            REQUIRE(set == std::vector<int> {4, 3, 2, 1, 0, 1, 2, 3, 4});
        }
    }
}

SCENARIO("Numbers::Cycle: Set params: with last returned number")
{
    using namespace aleatoric;

    Range range(1, 3);

    GIVEN("State set to unidirectional, forward direction")
    {
        Cycle instance(range, true,
                       true); // make it start with different state

        auto lastNumber = instance.getIntegerNumber(); // 3

        bool newBidirectional = false;
        bool newReverseDirection = false;

        WHEN("Last number is mid new range")
        {
            Range newRange(lastNumber - 1, lastNumber + 1); // 2 - 4
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 2, 3, 4, 2, 3});
            }
        }

        WHEN("Last number is at end of new range")
        {
            Range newRange(lastNumber - 3, lastNumber); // 0 - 3
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {0, 1, 2, 3, 0, 1, 2, 3});
            }
        }

        WHEN("Last number is at start of new range")
        {
            Range newRange(lastNumber, lastNumber + 2); // 3 - 5
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 5, 3, 4, 5, 3});
            }
        }

        WHEN("Last number not in new range")
        {
            Range newRange(lastNumber + 1, lastNumber + 3); // 4 - 6
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("Cycle jumps to start of new range")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 5, 6, 4, 5, 6});
            }
        }
    }

    GIVEN("State set to unidirectional, reverse direction")
    {
        Cycle instance(range,
                       true,
                       false); // make it start with different state

        auto lastNumber = instance.getIntegerNumber(); // 1

        bool newBidirectional = false;
        bool newReverseDirection = true;

        WHEN("Last number is mid new range")
        {
            Range newRange(lastNumber - 1, lastNumber + 1); // 0 - 2
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {0, 2, 1, 0, 2, 1});
            }
        }

        WHEN("Last number is at end of new range")
        {
            Range newRange(lastNumber - 2, lastNumber); // -1, 1
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {0, -1, 1, 0, -1, 1});
            }
        }

        WHEN("Last number is at start of new range")
        {
            Range newRange(lastNumber, lastNumber + 3); // 1 - 4
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 3, 2, 1, 4, 3, 2, 1});
            }
        }

        WHEN("Last number not in new range")
        {
            Range newRange(lastNumber + 1, lastNumber + 3); // 2 - 4
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("Cycle jumps to end of new range")
            {
                std::vector<int> set(newRange.size * 2);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 3, 2, 4, 3, 2});
            }
        }
    }

    GIVEN("State set to bidirectional, forward direction")
    {
        Cycle instance(range,
                       false,
                       true); // make it start with different state

        auto lastNumber = instance.getIntegerNumber(); // 3

        bool newBidirectional = true;
        bool newReverseDirection = false;

        WHEN("Last number is mid new range")
        {
            Range newRange(lastNumber - 1, lastNumber + 1); // 2 - 4
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 3, 2, 3, 4});
            }
        }

        WHEN("Last number is at end of new range")
        {
            Range newRange(lastNumber - 3, lastNumber); // 0, 3
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {2, 1, 0, 1, 2, 3, 2});
            }
        }

        WHEN("Last number is at start of new range")
        {
            Range newRange(lastNumber, lastNumber + 2); // 3 - 5
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 5, 4, 3, 4});
            }
        }

        WHEN("Last number not in new range")
        {
            Range newRange(lastNumber + 1, lastNumber + 3); // 4 - 6
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("Cycle jumps to start of new range")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 5, 6, 5, 4});
            }
        }
    }

    GIVEN("State set to bidirectional, reverse direction")
    {
        Cycle instance(range,
                       false,
                       false); // make it start with different state

        auto lastNumber = instance.getIntegerNumber(); // 1

        bool newBidirectional = true;
        bool newReverseDirection = true;

        WHEN("Last number is mid new range")
        {
            Range newRange(lastNumber - 1, lastNumber + 1); // 0 - 2
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {0, 1, 2, 1, 0});
            }
        }

        WHEN("Last number is at end of new range")
        {
            Range newRange(lastNumber - 2, lastNumber); // -1, 1
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {0, -1, 0, 1, 0});
            }
        }

        WHEN("Last number is at start of new range")
        {
            Range newRange(lastNumber, lastNumber + 3); // 1 - 4
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("The cycle continues as expected")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {2, 3, 4, 3, 2, 1, 2});
            }
        }

        WHEN("Last number not in new range")
        {
            Range newRange(lastNumber + 1, lastNumber + 3); // 2 - 4
            NumberProtocolParameters newParams(
                newRange,
                NumberProtocolParameters::Protocols(
                    NumberProtocolParameters::Cycle(newBidirectional,
                                                    newReverseDirection)));
            instance.setParams(newParams);

            THEN("Cycle jumps to end of new range")
            {
                std::vector<int> set(newRange.size * 2 - 1);
                for(auto &&i : set) {
                    i = instance.getIntegerNumber();
                }

                REQUIRE(set == std::vector<int> {4, 3, 2, 3, 4});
            }
        }
    }
}
