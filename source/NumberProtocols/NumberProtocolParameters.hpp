#ifndef NumberProtocolParameters_hpp
#define NumberProtocolParameters_hpp

#include "NumberProtocol.hpp"
#include "Range.hpp"

#include <vector>

namespace aleatoric {

struct NumberProtocolParams; // forward declaration for setting up friend
                             // relationships

struct AdjacentStepsParams {};

struct BasicParams {};

struct CycleParams {
    CycleParams(bool bidirectional, bool reverseDirection);
    friend struct NumberProtocolParams;
    bool getBidirectional();
    bool getReverseDirection();

  private:
    CycleParams();
    bool m_bidirectional = false;
    bool m_reverseDirection = false;
};

struct GranularWalkParams {
    GranularWalkParams(double deviationFactor);
    friend struct NumberProtocolParams;
    double getDeviationFactor();

  private:
    GranularWalkParams();
    double m_deviationFactor = 1.0;
};

struct GroupedRepetitionParams {
    GroupedRepetitionParams(std::vector<int> groupings);
    friend struct NumberProtocolParams;
    std::vector<int> getGroupings();

  private:
    GroupedRepetitionParams();
    std::vector<int> m_groupings {1};
};

struct NoRepetitionParams {};

struct PeriodicParams {
    PeriodicParams(double chanceOfRepetition);
    friend struct NumberProtocolParams;
    double getChanceOfRepetition();

  private:
    PeriodicParams();
    double m_chanceOfRepetition = 0.0;
};

struct PrecisionParams {
    PrecisionParams(std::vector<double> distribution);
    friend struct NumberProtocolParams;
    std::vector<double> getDistribution();

  private:
    PrecisionParams();
    std::vector<double> m_distribution {};
};

struct RatioParams {
    RatioParams(std::vector<int> ratios);
    friend struct NumberProtocolParams;
    std::vector<int> getRatios();

  private:
    RatioParams();
    std::vector<int> m_ratios {};
};

struct SerialParams {};

struct SubsetParams {
    SubsetParams(int min, int max);
    friend struct NumberProtocolParams;
    int getMin();
    int getMax();

  private:
    SubsetParams();
    int m_min = 0;
    int m_max = 0;
};

struct WalkParams {
    WalkParams(int maxStep);
    friend struct NumberProtocolParams;
    int getMaxStep();

  private:
    WalkParams();
    int m_maxStep = 1;
};

struct NumberProtocolParams {
    friend struct NumberProtocolConfig;

    NumberProtocolParams(AdjacentStepsParams protocolParams);
    NumberProtocolParams(BasicParams protocolParams);
    NumberProtocolParams(CycleParams protocolParams);
    NumberProtocolParams(GranularWalkParams protocolParams);
    NumberProtocolParams(GroupedRepetitionParams protocolParams);
    NumberProtocolParams(NoRepetitionParams protocolParams);
    NumberProtocolParams(PeriodicParams protocolParams);
    NumberProtocolParams(PrecisionParams protocolParams);
    NumberProtocolParams(RatioParams protocolParams);
    NumberProtocolParams(SerialParams protocolParams);
    NumberProtocolParams(SubsetParams protocolParams);
    NumberProtocolParams(WalkParams protocolParams);

    NumberProtocol::Type getActiveProtocol();
    AdjacentStepsParams getAdjacentSteps();
    BasicParams getBasic();
    CycleParams getCycle();
    GranularWalkParams getGranularWalk();
    GroupedRepetitionParams getGroupedRepetition();
    NoRepetitionParams getNoRepetition();
    PeriodicParams getPeriodic();
    PrecisionParams getPrecision();
    RatioParams getRatio();
    SerialParams getSerial();
    SubsetParams getSubset();
    WalkParams getWalk();

  private:
    NumberProtocolParams();
    NumberProtocol::Type m_activeProtocol = NumberProtocol::Type::none;

    AdjacentStepsParams m_adjacentSteps;
    BasicParams m_basic;
    CycleParams m_cycle;
    GranularWalkParams m_granularWalk;
    GroupedRepetitionParams m_groupedRepetition;
    NoRepetitionParams m_noRepetition;
    PeriodicParams m_periodic;
    PrecisionParams m_precision;
    RatioParams m_ratio;
    SerialParams m_serial;
    SubsetParams m_subset;
    WalkParams m_walk;
};

struct NumberProtocolConfig {
    NumberProtocolConfig(Range newRange, NumberProtocolParams protocolsParams);

    Range getRange();

    NumberProtocolParams protocols;

    template<typename>
    friend class CollectionsProducer;

    friend class DurationsProducer;

  private:
    NumberProtocolConfig(Range newRange);
    Range m_range = Range(0, 1);
};

} // namespace aleatoric

#endif /* NumberProtocolParameters_hpp */
