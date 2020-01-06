#ifndef Periodic_hpp
#define Periodic_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolInterface.hpp"
#include "Range.hpp"

// TODO: documentation

namespace actlib { namespace Numbers {
class Periodic : public Protocol {
  public:
    Periodic(IDiscreteGenerator &generator,
             Range &range,
             double chanceOfRepetition);

    Periodic(IDiscreteGenerator &generator,
             Range &range,
             double chanceOfRepetition,
             int initialSelection);

    ~Periodic();

    int getNumber() override;
    void reset() override;

  private:
    Range &m_range;
    IDiscreteGenerator &m_generator;
    double m_periodicity;
    double m_remainderAllocation;
    void setPeriodicDistribution(int selectedIndex);
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
}} // namespace actlib::Numbers

#endif /* Periodic_hpp */
