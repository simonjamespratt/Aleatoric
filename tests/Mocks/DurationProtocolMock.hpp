#ifndef DurationProtocolMock_hpp
#define DurationProtocolMock_hpp

#include "DurationProtocol.hpp"

#include <catch2/catch.hpp>
#include <catch2/trompeloeil.hpp>

class DurationProtocolMock : public aleatoric::DurationProtocol {
  public:
    MAKE_MOCK0(getCollectionSize, int());
    MAKE_MOCK1(getDuration, int(int));
};

#endif /* DurationProtocolMock_hpp */
