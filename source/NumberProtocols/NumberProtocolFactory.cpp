#include "NumberProtocolFactory.hpp"

#include "AdjacentSteps.hpp"
#include "Basic.hpp"
#include "Cycle.hpp"
#include "DiscreteGenerator.hpp"
#include "GranularWalk.hpp"
#include "GroupedRepetition.hpp"
#include "NoRepetition.hpp"
#include "Periodic.hpp"
#include "Precision.hpp"
#include "Range.hpp"
#include "Ratio.hpp"
#include "Serial.hpp"
#include "Subset.hpp"
#include "UniformGenerator.hpp"
#include "Walk.hpp"

namespace aleatoric {
NumberProtocolFactory::NumberProtocolFactory() {};
NumberProtocolFactory::~NumberProtocolFactory() {};

std::unique_ptr<NumberProtocol>
NumberProtocolFactory::createAdjacentSteps(int rangeStart, int rangeEnd)
{
    return std::make_unique<AdjacentSteps>(
        std::make_unique<DiscreteGenerator>(),
        Range(rangeStart, rangeEnd));
}

std::unique_ptr<NumberProtocol>
NumberProtocolFactory::createBasic(int rangeStart, int rangeEnd)
{
    return std::make_unique<Basic>(std::make_unique<UniformGenerator>(),
                                   Range(rangeStart, rangeEnd));
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createCycle(
    int rangeStart, int rangeEnd, bool bidirectional, bool reverseDirection)
{
    return std::make_unique<Cycle>(Range(rangeStart, rangeEnd),
                                   bidirectional,
                                   reverseDirection);
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createGroupedRepetition(
    int rangeStart, int rangeEnd, std::vector<int> groupings)
{
    return std::make_unique<GroupedRepetition>(
        std::make_unique<DiscreteGenerator>(),
        std::make_unique<DiscreteGenerator>(),
        Range(rangeStart, rangeEnd),
        groupings);
}

std::unique_ptr<NumberProtocol>
NumberProtocolFactory::createNoRepetition(int rangeStart, int rangeEnd)
{
    return std::make_unique<NoRepetition>(std::make_unique<DiscreteGenerator>(),
                                          Range(rangeStart, rangeEnd));
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createPeriodic(
    int rangeStart, int rangeEnd, double chanceOfRepetition)
{
    return std::make_unique<Periodic>(std::make_unique<DiscreteGenerator>(),
                                      Range(rangeStart, rangeEnd),
                                      chanceOfRepetition);
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createPrecision(
    int rangeStart, int rangeEnd, std::vector<double> distribution)
{
    return std::make_unique<Precision>(std::make_unique<DiscreteGenerator>(),
                                       Range(rangeStart, rangeEnd),
                                       distribution);
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createRatio(
    int rangeStart, int rangeEnd, std::vector<int> ratios)
{
    return std::make_unique<Ratio>(std::make_unique<DiscreteGenerator>(),
                                   Range(rangeStart, rangeEnd),
                                   ratios);
}

std::unique_ptr<NumberProtocol>
NumberProtocolFactory::createSerial(int rangeStart, int rangeEnd)
{
    return std::make_unique<Serial>(std::make_unique<DiscreteGenerator>(),
                                    Range(rangeStart, rangeEnd));
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createSubset(
    int rangeStart, int rangeEnd, int subsetMin, int subsetMax)
{
    return std::make_unique<Subset>(std::make_unique<UniformGenerator>(),
                                    std::make_unique<DiscreteGenerator>(),
                                    Range(rangeStart, rangeEnd),
                                    subsetMin,
                                    subsetMax);
}

std::unique_ptr<NumberProtocol>
NumberProtocolFactory::createWalk(int rangeStart, int rangeEnd, int maxStep)
{
    return std::make_unique<Walk>(std::make_unique<UniformGenerator>(),
                                  Range(rangeStart, rangeEnd),
                                  maxStep);
}

std::unique_ptr<NumberProtocol> NumberProtocolFactory::createGranularWalk(
    int rangeStart, int rangeEnd, double deviationFactor)
{
    return std::make_unique<GranularWalk>(std::make_unique<UniformGenerator>(),
                                          Range(rangeStart, rangeEnd),
                                          deviationFactor);
}
} // namespace aleatoric
