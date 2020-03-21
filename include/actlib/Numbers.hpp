#ifndef Numbers_hpp
#define Numbers_hpp

#include "ProtocolGranular.hpp"
#include "ProtocolSteps.hpp"

#include <memory>

namespace actlib { namespace Numbers {
class Numbers {
  public:
    // TODO: Ideally would like to condense the following into one method with
    // an argument that specifies the protocol to create. However, at the moment
    // there is no way to standardise the params that would need to be passed to
    // one method. There is a pitch doc about updating protocol parameters (at
    // runtime) which, when implemented, would potentially allow this to happen.

    Numbers();

    ~Numbers();

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createAdjacentSteps(int rangeStart, int rangeEnd);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createAdjacentSteps(int rangeStart, int rangeEnd, int initialSelection);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createBasic(int rangeStart, int rangeEnd);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createNoRepetition(int rangeStart, int rangeEnd);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createPeriodic(int rangeStart, int rangeEnd, double chanceOfRepetition);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createPeriodic(int rangeStart,
                   int rangeEnd,
                   double chanceOfRepetition,
                   int initialSelection);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createSerial(int rangeStart, int rangeEnd);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createWalk(int rangeStart, int rangeEnd, int maxStep);

    std::unique_ptr<actlib::Numbers::Steps::Protocol>
    createWalk(int rangeStart, int rangeEnd, int maxStep, int initialSelection);

    std::unique_ptr<actlib::Numbers::Granular::Protocol>
    createGranularWalk(int rangeStart, int rangeEnd, double deviationFactor);

    std::unique_ptr<actlib::Numbers::Granular::Protocol>
    createGranularWalk(int rangeStart,
                       int rangeEnd,
                       double deviationFactor,
                       int initialSelection);
};
}} // namespace actlib::Numbers

#endif /* Numbers_hpp */
