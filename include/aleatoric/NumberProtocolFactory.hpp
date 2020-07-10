#ifndef NumberProtocolFactory_hpp
#define NumberProtocolFactory_hpp

#include "NumberProtocol.hpp"

#include <memory>

namespace aleatoric {
class NumberProtocolFactory {
  public:
    // TODO: DYNAMIC-PARAMS: Ideally would like to condense the following into
    // one method with an argument that specifies the protocol to create.
    // However, at the moment there is no way to standardise the params that
    // would need to be passed to one method. There is a pitch doc about
    // updating protocol parameters (at runtime) which, when implemented, would
    // potentially allow this to happen.

    NumberProtocolFactory();

    ~NumberProtocolFactory();

    std::unique_ptr<NumberProtocol> createAdjacentSteps(int rangeStart,
                                                        int rangeEnd);

    std::unique_ptr<NumberProtocol>
    createAdjacentSteps(int rangeStart, int rangeEnd, int initialSelection);

    std::unique_ptr<NumberProtocol> createBasic(int rangeStart, int rangeEnd);

    std::unique_ptr<NumberProtocol> createCycle(int rangeStart,
                                                int rangeEnd,
                                                bool bidirectional = false,
                                                bool reverseDirection = false);

    std::unique_ptr<NumberProtocol> createCycle(int rangeStart,
                                                int rangeEnd,
                                                int initialSelection,
                                                bool bidirectional = false,
                                                bool reverseDirection = false);

    std::unique_ptr<NumberProtocol> createGroupedRepetition(
        int rangeStart, int rangeEnd, std::vector<int> groupings);

    std::unique_ptr<NumberProtocol> createNoRepetition(int rangeStart,
                                                       int rangeEnd);

    std::unique_ptr<NumberProtocol>
    createPeriodic(int rangeStart, int rangeEnd, double chanceOfRepetition);

    std::unique_ptr<NumberProtocol> createPeriodic(int rangeStart,
                                                   int rangeEnd,
                                                   double chanceOfRepetition,
                                                   int initialSelection);

    std::unique_ptr<NumberProtocol> createPrecision(
        int rangeStart, int rangeEnd, std::vector<double> distribution);

    std::unique_ptr<NumberProtocol>
    createPrecision(int rangeStart,
                    int rangeEnd,
                    std::vector<double> distribution,
                    int initialSelection);

    std::unique_ptr<NumberProtocol>
    createRatio(int rangeStart, int rangeEnd, std::vector<int> ratios);

    std::unique_ptr<NumberProtocol> createSerial(int rangeStart, int rangeEnd);

    std::unique_ptr<NumberProtocol>
    createSubset(int rangeStart, int rangeEnd, int subsetMin, int subsetMax);

    std::unique_ptr<NumberProtocol>
    createWalk(int rangeStart, int rangeEnd, int maxStep);

    std::unique_ptr<NumberProtocol>
    createWalk(int rangeStart, int rangeEnd, int maxStep, int initialSelection);

    std::unique_ptr<NumberProtocol>
    createGranularWalk(int rangeStart, int rangeEnd, double deviationFactor);

    std::unique_ptr<NumberProtocol> createGranularWalk(int rangeStart,
                                                       int rangeEnd,
                                                       double deviationFactor,
                                                       int initialSelection);
};
} // namespace aleatoric

#endif /* NumberProtocolFactory_hpp */
