#include "DiscreteGenerator.hpp"

#include <catch2/catch.hpp>

SCENARIO("DiscreteGenerator")
{
    GIVEN("The class is constructed with a vector")
    {
        std::vector<double> distributionVector = {0.0, 1.0};
        actlib::Numbers::DiscreteGenerator instance(distributionVector);

        WHEN("The distribution vector is requested")
        {
            THEN("The result should match the vector received upon "
                 "construction")
            {
                REQUIRE(instance.getDistributionVector() == distributionVector);
            }
        }

        WHEN("A number is requested")
        {
            THEN("It should return the expected number")
            {
                // NB: This is a pseudo test, in that it is unlikely to be
                // wrong, but is not guaranteed to be right! In order to test
                // this properly it would require understanding how to
                // deterministically seed the RNG or Mock things (which I don't
                // think is possible)
                for(int i = 0; i < 100; i++) {
                    REQUIRE(instance.getNumber() == 1);
                }
            }
        }
    }

    GIVEN("The class is constructed with a vectorSize and uniformValue")
    {
        actlib::Numbers::DiscreteGenerator instance(3, 1.0);

        WHEN("The distribution vector is requested")
        {
            THEN("The result should match the required vector formation "
                 "requested at construction")
            {
                REQUIRE(instance.getDistributionVector() ==
                        std::vector<double> {1.0, 1.0, 1.0});
            }
        }

        WHEN("A number is requested")
        {
            THEN("It should return an expected number")
            {
                // NB: This is a pseudo test, in that it is unlikely to be
                // wrong, but is not guaranteed to be right! In order to test
                // this properly it would require understanding how to
                // deterministically seed the RNG or Mock things (which I don't
                // think is possible)
                for(int i = 0; i < 100; i++) {
                    REQUIRE(instance.getNumber() >= 0);
                    REQUIRE(instance.getNumber() <= 2);
                }
            }
        }
    }

    GIVEN("[setDistributionVector] An instance of the class exists")
    {
        // NB: The following tests deliberately do NOT check the results of
        // calling getNumber() after vector manipulation They could be added if
        // it is really desired / needed

        actlib::Numbers::DiscreteGenerator instance(std::vector<double> {});

        WHEN("The distribution vector is (re)set by providing a new vector")
        {
            std::vector<double> newVector = {1.0};
            instance.setDistributionVector(newVector);

            AND_WHEN("The distribution vector is requested")
            {
                THEN("The result should match the new vector received")
                {
                    REQUIRE(instance.getDistributionVector() == newVector);
                }
            }
        }

        WHEN("The distribution vector is (re)set by providing a vectorSize and "
             "uniformValue")
        {
            instance.setDistributionVector(3, 1.0);

            AND_WHEN("The distribution vector is requested")
            {
                THEN("The result should match the new vector received")
                {
                    REQUIRE(instance.getDistributionVector() ==
                            std::vector<double> {1.0, 1.0, 1.0});
                }
            }
        }
    }

    GIVEN("[updateDistributionVector] An instance of the class exists")
    {
        // NB: The following tests deliberately do NOT check the results of
        // calling getNumber() after vector manipulation They could be added if
        // it is really desired / needed

        WHEN("The distribution vector is updated by changing a specific item "
             "in the vector")
        {
            actlib::Numbers::DiscreteGenerator instance(
                std::vector<double> {1.0, 1.0});
            instance.updateDistributionVector(1, 0.0);

            AND_WHEN("The distribution vector is requested")
            {
                THEN("The result should reflect the update made")
                {
                    REQUIRE(instance.getDistributionVector() ==
                            std::vector<double> {1.0, 0.0});
                }
            }
        }

        WHEN("The distribution vector is updated by setting all vector items "
             "uniformly")
        {
            actlib::Numbers::DiscreteGenerator instance(
                std::vector<double> {1.0, 1.0});
            instance.updateDistributionVector(0.0);

            AND_WHEN("The distribution vector is requested")
            {
                THEN("The result should reflect the update made")
                {
                    REQUIRE(instance.getDistributionVector() ==
                            std::vector<double> {0.0, 0.0});
                }
            }
        }
    }
}
