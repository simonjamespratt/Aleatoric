#include "CollectionsProducer.hpp"

#include "NumberProtocolFactory.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

SCENARIO("Collections::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        aleatoric::NumberProtocolFactory factory;

        std::vector<char> sourceCollection {'a', 'b', 'c'};

        aleatoric::CollectionsProducer<char> instance(
            sourceCollection,
            factory.createBasic(0, 1));

        WHEN("An item is requested")
        {
            THEN("It should be an item from the source collection")
            {
                auto returnedItem = instance.getItem();
                REQUIRE_THAT(sourceCollection,
                             Catch::VectorContains(returnedItem));
            }
        }

        WHEN("A collection of selections is requested")
        {
            auto set = instance.getCollection(1000);

            THEN("The size of the set should match that requested")
            {
                REQUIRE(set.size() == 1000);
            }

            THEN("Each item in the set should be in the source collection")
            {
                for(auto &&i : set) {
                    REQUIRE_THAT(sourceCollection, Catch::VectorContains(i));
                }
            }

            THEN("Each item in the source collection should be present in the "
                 "set")
            {
                for(auto &&i : sourceCollection) {
                    REQUIRE_THAT(set, Catch::VectorContains(i));
                }
            }
        }

        // TODO: RESET-TEST-FAIL: Reinstate this test - it is failing for
        // unknown reasons - same as for ProducerGranularTest WHEN("A reset is
        // requested")
        // {
        //     THEN("It should call the given protocol reset method")
        //     {
        //         REQUIRE_CALL(*protocolPointer, reset());
        //         instance.reset();
        //     }
        // }
    }
}
