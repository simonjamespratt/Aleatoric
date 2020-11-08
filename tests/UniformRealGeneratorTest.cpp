#include "UniformRealGenerator.hpp"

#include <catch2/catch.hpp>

SCENARIO("UniformRealGenerator: default constructor")
{
    using namespace aleatoric;

    UniformRealGenerator instance;

    THEN("Returned range is 0-1")
    {
        auto distribution = instance.getDistribution();
        REQUIRE(distribution.first == 0.0);
        REQUIRE(distribution.second == 1.0);
    }

    THEN("Returned numbers are within the default range")
    {
        for(int i = 0; i < 10000; i++) {
            auto number = instance.getNumber();
            REQUIRE((number >= 0.0 && number <= 1.0));
        }
    }

    WHEN("Set distribution range")
    {
        std::pair<double, double> newRange(33.33, 66.66);
        instance.setDistribution(newRange.first, newRange.second);

        THEN("Range is updated")
        {
            auto distribution = instance.getDistribution();
            REQUIRE(distribution.first == newRange.first);
            REQUIRE(distribution.second == newRange.second);
        }

        THEN("All numbers returned are within new range")
        {
            for(int i = 0; i < 10000; i++) {
                auto number = instance.getNumber();
                REQUIRE(
                    (number >= newRange.first && number <= newRange.second));
            }
        }
    }
}

SCENARIO("UniformRealGenerator: constructed with range params")
{
    using namespace aleatoric;

    double rangeStart = 33.33;
    double rangeEnd = 66.66;
    UniformRealGenerator instance(rangeStart, rangeEnd);

    THEN("Returned range is as supplied")
    {
        auto distribution = instance.getDistribution();
        REQUIRE(distribution.first == rangeStart);
        REQUIRE(distribution.second == rangeEnd);
    }

    THEN("Numbers returned are within provided range")
    {
        for(int i = 0; i < 10000; i++) {
            auto number = instance.getNumber();
            REQUIRE((number >= rangeStart && number <= rangeEnd));
        }
    }
}
