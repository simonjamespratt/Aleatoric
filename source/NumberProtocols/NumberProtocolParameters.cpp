#include "NumberProtocolParameters.hpp"

namespace aleatoric {

NumberProtocolConfig::NumberProtocolConfig(Range newRange,
                                           NumberProtocolParams protocolsParams)
{
    m_range = newRange;
    protocols = protocolsParams;
}

NumberProtocolConfig::NumberProtocolConfig(Range newRange)
{
    m_range = newRange;
    protocols.m_subset = SubsetParams(1, newRange.size);

    std::vector<int> ratios(newRange.size);
    for(auto &&i : ratios) {
        i = 1;
    }
    protocols.m_ratio = RatioParams(ratios);

    std::vector<double> distribution(newRange.size);
    for(auto &&i : distribution) {
        i = 1.0 / distribution.size();
    }
    protocols.m_precision = PrecisionParams(distribution);
}

Range NumberProtocolConfig::getRange()
{
    return m_range;
}

// Protocols =====================================================

// Constructors
NumberProtocolParams::NumberProtocolParams()
{}

NumberProtocolParams::NumberProtocolParams(AdjacentStepsParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::adjacentSteps;
    m_adjacentSteps = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(BasicParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::basic;
    m_basic = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(CycleParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::cycle;
    m_cycle = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(GranularWalkParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::granularWalk;
    m_granularWalk = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(
    GroupedRepetitionParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::groupedRepetition;
    m_groupedRepetition = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(NoRepetitionParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::noRepetition;
    m_noRepetition = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(PeriodicParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::periodic;
    m_periodic = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(PrecisionParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::precision;
    m_precision = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(RatioParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::ratio;
    m_ratio = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(SerialParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::serial;
    m_serial = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(SubsetParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::subset;
    m_subset = protocolParams;
}

NumberProtocolParams::NumberProtocolParams(WalkParams protocolParams)
{
    m_activeProtocol = NumberProtocol::Type::walk;
    m_walk = protocolParams;
}

// other methods

NumberProtocol::Type NumberProtocolParams::getActiveProtocol()
{
    return m_activeProtocol;
}

AdjacentStepsParams NumberProtocolParams::getAdjacentSteps()
{
    return m_adjacentSteps;
}

BasicParams NumberProtocolParams::getBasic()
{
    return m_basic;
}

CycleParams NumberProtocolParams::getCycle()
{
    return m_cycle;
}

GranularWalkParams NumberProtocolParams::getGranularWalk()
{
    return m_granularWalk;
}

GroupedRepetitionParams NumberProtocolParams::getGroupedRepetition()
{
    return m_groupedRepetition;
}

NoRepetitionParams NumberProtocolParams::getNoRepetition()
{
    return m_noRepetition;
}

PeriodicParams NumberProtocolParams::getPeriodic()
{
    return m_periodic;
}

PrecisionParams NumberProtocolParams::getPrecision()
{
    return m_precision;
}

RatioParams NumberProtocolParams::getRatio()
{
    return m_ratio;
}

SerialParams NumberProtocolParams::getSerial()
{
    return m_serial;
}

SubsetParams NumberProtocolParams::getSubset()
{
    return m_subset;
}

WalkParams NumberProtocolParams::getWalk()
{
    return m_walk;
}

// ===============================================================

// Cycle
CycleParams::CycleParams()
{}

CycleParams::CycleParams(bool bidirectional, bool reverseDirection)
{
    m_bidirectional = bidirectional;
    m_reverseDirection = reverseDirection;
}

bool CycleParams::getBidirectional()
{
    return m_bidirectional;
}

bool CycleParams::getReverseDirection()
{
    return m_reverseDirection;
}

// GranularWalk
GranularWalkParams::GranularWalkParams()
{}

GranularWalkParams::GranularWalkParams(double deviationFactor)
{
    m_deviationFactor = deviationFactor;
}

double GranularWalkParams::getDeviationFactor()
{
    return m_deviationFactor;
}

// Grouped Repetition
GroupedRepetitionParams::GroupedRepetitionParams()
{}

GroupedRepetitionParams::GroupedRepetitionParams(std::vector<int> groupings)
{
    m_groupings = groupings;
}

std::vector<int> GroupedRepetitionParams::getGroupings()
{
    return m_groupings;
}

// Periodic
PeriodicParams::PeriodicParams()
{}

PeriodicParams::PeriodicParams(double chanceOfRepetition)
{
    m_chanceOfRepetition = chanceOfRepetition;
}

double PeriodicParams::getChanceOfRepetition()
{
    return m_chanceOfRepetition;
}

// Precision
PrecisionParams::PrecisionParams()
{}

PrecisionParams::PrecisionParams(std::vector<double> distribution)
{
    m_distribution = distribution;
}

std::vector<double> PrecisionParams::getDistribution()
{
    return m_distribution;
}

// Ratio
RatioParams::RatioParams()
{}

RatioParams::RatioParams(std::vector<int> ratios)
{
    m_ratios = ratios;
}

std::vector<int> RatioParams::getRatios()
{
    return m_ratios;
}

// Subset
SubsetParams::SubsetParams()
{}

SubsetParams::SubsetParams(int min, int max)
{
    m_min = min;
    m_max = max;
}

int SubsetParams::getMin()
{
    return m_min;
}

int SubsetParams::getMax()
{
    return m_max;
}

// Walk
WalkParams::WalkParams()
{}

WalkParams::WalkParams(int maxStep)
{
    m_maxStep = maxStep;
}

int WalkParams::getMaxStep()
{
    return m_maxStep;
}
} // namespace aleatoric
