#ifndef NumberProtocolParameters_hpp
#define NumberProtocolParameters_hpp

#include "Range.hpp"

#include <vector>

namespace aleatoric {

struct NumberProtocolParameters {
    struct Protocols; // forward declaration for setting up friend relationships

    struct AdjacentSteps {};

    struct Basic {};

    struct Cycle {
        Cycle(bool bidirectional, bool reverseDirection);
        friend struct Protocols;
        bool getBidirectional();
        bool getReverseDirection();

      private:
        Cycle();
        bool m_bidirectional = false;
        bool m_reverseDirection = false;
    };

    struct GranularWalk {
        GranularWalk(double deviationFactor);
        friend struct Protocols;
        double getDeviationFactor();

      private:
        GranularWalk();
        double m_deviationFactor = 1.0;
    };

    struct GroupedRepetition {
        GroupedRepetition(std::vector<int> groupings);
        friend struct Protocols;
        std::vector<int> getGroupings();

      private:
        GroupedRepetition();
        std::vector<int> m_groupings {1};
    };

    struct NoRepetition {};

    struct Periodic {
        Periodic(double chanceOfRepetition);
        friend struct Protocols;
        double getChanceOfRepetition();

      private:
        Periodic();
        double m_chanceOfRepetition = 0.0;
    };

    struct Precision {
        Precision(std::vector<double> distribution);
        friend struct Protocols;
        std::vector<double> getDistribution();

      private:
        Precision();
        std::vector<double> m_distribution {};
    };

    struct Ratio {
        Ratio(std::vector<int> ratios);
        friend struct Protocols;
        std::vector<int> getRatios();

      private:
        Ratio();
        std::vector<int> m_ratios {};
    };

    struct Serial {};

    struct Subset {
        Subset(int min, int max);
        friend struct Protocols;
        int getMin();
        int getMax();

      private:
        Subset();
        int m_min = 0;
        int m_max = 0;
    };

    struct Walk {
        Walk(int maxStep);
        friend struct Protocols;
        int getMaxStep();

      private:
        Walk();
        int m_maxStep = 1;
    };

    struct Protocols {
        enum class ActiveProtocol {
            adjacentSteps,
            basic,
            cycle,
            granularWalk,
            groupedRepetition,
            noRepetition,
            periodic,
            precision,
            ratio,
            serial,
            subset,
            walk,
            none
        };

        friend struct NumberProtocolParameters;

        Protocols(AdjacentSteps protocolParams);
        Protocols(Basic protocolParams);
        Protocols(Cycle protocolParams);
        Protocols(GranularWalk protocolParams);
        Protocols(GroupedRepetition protocolParams);
        Protocols(NoRepetition protocolParams);
        Protocols(Periodic protocolParams);
        Protocols(Precision protocolParams);
        Protocols(Ratio protocolParams);
        Protocols(Serial protocolParams);
        Protocols(Subset protocolParams);
        Protocols(Walk protocolParams);

        ActiveProtocol getActiveProtocol();
        AdjacentSteps getAdjacentSteps();
        Basic getBasic();
        Cycle getCycle();
        GranularWalk getGranularWalk();
        GroupedRepetition getGroupedRepetition();
        NoRepetition getNoRepetition();
        Periodic getPeriodic();
        Precision getPrecision();
        Ratio getRatio();
        Serial getSerial();
        Subset getSubset();
        Walk getWalk();

      private:
        Protocols();
        ActiveProtocol m_activeProtocol = ActiveProtocol::none;

        AdjacentSteps m_adjacentSteps;
        Basic m_basic;
        Cycle m_cycle;
        GranularWalk m_granularWalk;
        GroupedRepetition m_groupedRepetition;
        NoRepetition m_noRepetition;
        Periodic m_periodic;
        Precision m_precision;
        Ratio m_ratio;
        Serial m_serial;
        Subset m_subset;
        Walk m_walk;
    };

    NumberProtocolParameters(Range newRange, Protocols protocolsParams);

    Range getRange();

    Protocols protocols;

    template<typename>
    friend class CollectionsProducer;

    friend class DurationsProducer;

  private:
    NumberProtocolParameters(Range newRange);
    Range m_range = Range(0, 1);
};

} // namespace aleatoric

#endif /* NumberProtocolParameters_hpp */
