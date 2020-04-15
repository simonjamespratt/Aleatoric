#include "ProducerCollections.hpp"

#include "ProtocolSteps.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class ConcreteProtocolMock : public actlib::Numbers::Steps::Protocol {
  public:
    MAKE_MOCK0(getNumber, int());
    MAKE_MOCK0(reset, void());
};

SCENARIO("Collections::Producer")
{
    GIVEN("The class is instantiated correctly")
    {
        std::vector<char> sourceCollection {'a', 'b', 'c'};

        auto protocol = std::make_unique<ConcreteProtocolMock>();
        auto protocolPointer = protocol.get();

        actlib::Collections::Producer<char> instance(sourceCollection,
                                                     std::move(protocol));

        WHEN("An item is requested")
        {
            THEN("It should return the item from the collection provided that "
                 "matches the number obtained from the protocol")
            {
                int acquiredNumber = 1;
                REQUIRE_CALL(*protocolPointer, getNumber())
                    .RETURN(acquiredNumber);

                auto returnedItem = instance.getItem();
                REQUIRE(returnedItem == sourceCollection[1]);
            }
        }

        WHEN("A collection of selections is requested")
        {
            THEN("It should return a collection of the specified size "
                 "contaning items whose indices match the results of calling "
                 "the protocol to get numbers")
            {
                int acquiredNumber = 1;
                REQUIRE_CALL(*protocolPointer, getNumber())
                    .TIMES(100)
                    .RETURN(acquiredNumber);

                auto returnedCollection = instance.getCollection(100);
                REQUIRE(returnedCollection.size() == 100);

                for(auto &&it : returnedCollection) {
                    REQUIRE(it == sourceCollection[acquiredNumber]);
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
