#include "NumberProtocolParameters.hpp"

namespace aleatoric {

NumberProtocolParameters::NumberProtocolParameters(Range newRange,
                                                   Protocols protocolsParams)
{
    m_range = newRange;
    protocols = protocolsParams;
}

NumberProtocolParameters::NumberProtocolParameters(Range newRange)
{
    m_range = newRange;
    protocols.m_subset = Subset(1, newRange.size);

    std::vector<int> ratios(newRange.size);
    for(auto &&i : ratios) {
        i = 1;
    }
    protocols.m_ratio = Ratio(ratios);

    std::vector<double> distribution(newRange.size);
    for(auto &&i : distribution) {
        i = 1.0 / distribution.size();
    }
    protocols.m_precision = Precision(distribution);
}

Range NumberProtocolParameters::getRange()
{
    return m_range;
}

// Protocols =====================================================

// Constructors
NumberProtocolParameters::Protocols::Protocols()
{}

NumberProtocolParameters::Protocols::Protocols(AdjacentSteps protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::adjacentSteps;
    m_adjacentSteps = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Basic protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::basic;
    m_basic = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Cycle protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::cycle;
    m_cycle = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(GranularWalk protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::granularWalk;
    m_granularWalk = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(GroupedRepetition protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::groupedRepetition;
    m_groupedRepetition = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(NoRepetition protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::noRepetition;
    m_noRepetition = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Periodic protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::periodic;
    m_periodic = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Precision protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::precision;
    m_precision = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Ratio protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::ratio;
    m_ratio = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Serial protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::serial;
    m_serial = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Subset protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::subset;
    m_subset = protocolParams;
}

NumberProtocolParameters::Protocols::Protocols(Walk protocolParams)
{
    m_activeProtocol = Protocols::ActiveProtocol::walk;
    m_walk = protocolParams;
}

// other methods

NumberProtocolParameters::Protocols::ActiveProtocol
NumberProtocolParameters::Protocols::getActiveProtocol()
{
    return m_activeProtocol;
}

NumberProtocolParameters::AdjacentSteps
NumberProtocolParameters::Protocols::getAdjacentSteps()
{
    return m_adjacentSteps;
}

NumberProtocolParameters::Basic NumberProtocolParameters::Protocols::getBasic()
{
    return m_basic;
}

NumberProtocolParameters::Cycle NumberProtocolParameters::Protocols::getCycle()
{
    return m_cycle;
}

NumberProtocolParameters::GranularWalk
NumberProtocolParameters::Protocols::getGranularWalk()
{
    return m_granularWalk;
}

NumberProtocolParameters::GroupedRepetition
NumberProtocolParameters::Protocols::getGroupedRepetition()
{
    return m_groupedRepetition;
}

NumberProtocolParameters::NoRepetition
NumberProtocolParameters::Protocols::getNoRepetition()
{
    return m_noRepetition;
}

NumberProtocolParameters::Periodic
NumberProtocolParameters::Protocols::getPeriodic()
{
    return m_periodic;
}

NumberProtocolParameters::Precision
NumberProtocolParameters::Protocols::getPrecision()
{
    return m_precision;
}

NumberProtocolParameters::Ratio NumberProtocolParameters::Protocols::getRatio()
{
    return m_ratio;
}

NumberProtocolParameters::Serial
NumberProtocolParameters::Protocols::getSerial()
{
    return m_serial;
}

NumberProtocolParameters::Subset
NumberProtocolParameters::Protocols::getSubset()
{
    return m_subset;
}

NumberProtocolParameters::Walk NumberProtocolParameters::Protocols::getWalk()
{
    return m_walk;
}

// ===============================================================

// Cycle
NumberProtocolParameters::Cycle::Cycle()
{}

NumberProtocolParameters::Cycle::Cycle(bool bidirectional,
                                       bool reverseDirection)
{
    m_bidirectional = bidirectional;
    m_reverseDirection = reverseDirection;
}

bool NumberProtocolParameters::Cycle::getBidirectional()
{
    return m_bidirectional;
}

bool NumberProtocolParameters::Cycle::getReverseDirection()
{
    return m_reverseDirection;
}

// GranularWalk
NumberProtocolParameters::GranularWalk::GranularWalk()
{}

NumberProtocolParameters::GranularWalk::GranularWalk(double deviationFactor)
{
    m_deviationFactor = deviationFactor;
}

double NumberProtocolParameters::GranularWalk::getDeviationFactor()
{
    return m_deviationFactor;
}

// Grouped Repetition
NumberProtocolParameters::GroupedRepetition::GroupedRepetition()
{}

NumberProtocolParameters::GroupedRepetition::GroupedRepetition(
    std::vector<int> groupings)
{
    m_groupings = groupings;
}

std::vector<int> NumberProtocolParameters::GroupedRepetition::getGroupings()
{
    return m_groupings;
}

// Periodic
NumberProtocolParameters::Periodic::Periodic()
{}

NumberProtocolParameters::Periodic::Periodic(double chanceOfRepetition)
{
    m_chanceOfRepetition = chanceOfRepetition;
}

double NumberProtocolParameters::Periodic::getChanceOfRepetition()
{
    return m_chanceOfRepetition;
}

// Precision
NumberProtocolParameters::Precision::Precision()
{}

NumberProtocolParameters::Precision::Precision(std::vector<double> distribution)
{
    m_distribution = distribution;
}

std::vector<double> NumberProtocolParameters::Precision::getDistribution()
{
    return m_distribution;
}

// Ratio
NumberProtocolParameters::Ratio::Ratio()
{}

NumberProtocolParameters::Ratio::Ratio(std::vector<int> ratios)
{
    m_ratios = ratios;
}

std::vector<int> NumberProtocolParameters::Ratio::getRatios()
{
    return m_ratios;
}

// Subset
NumberProtocolParameters::Subset::Subset()
{}

NumberProtocolParameters::Subset::Subset(int min, int max)
{
    m_min = min;
    m_max = max;
}

int NumberProtocolParameters::Subset::getMin()
{
    return m_min;
}

int NumberProtocolParameters::Subset::getMax()
{
    return m_max;
}

// Walk
NumberProtocolParameters::Walk::Walk()
{}

NumberProtocolParameters::Walk::Walk(int maxStep)
{
    m_maxStep = maxStep;
}

int NumberProtocolParameters::Walk::getMaxStep()
{
    return m_maxStep;
}
} // namespace aleatoric
