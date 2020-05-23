#ifndef GroupedRepetition_hpp
#define GroupedRepetition_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolSteps.hpp"
#include "Range.hpp"
#include "Utilities.hpp"

namespace actlib { namespace Numbers { namespace Steps {
class GroupedRepetition : public Protocol {
  public:
    GroupedRepetition(std::unique_ptr<IDiscreteGenerator> numberGenerator,
                      std::unique_ptr<IDiscreteGenerator> groupingGenerator,
                      std::unique_ptr<Range> range,
                      std::vector<int> groupings);

    ~GroupedRepetition();
    int getNumber() override;
    void reset() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_numberGenerator;
    std::unique_ptr<IDiscreteGenerator> m_groupingGenerator;
    std::unique_ptr<Range> m_range;
    std::vector<int> m_groupings;
    actlib::Utilities::SeriesPrinciple m_seriesPrinciple;
    int m_groupingCount;
    int m_currentReturnableNumber;
};
}}}    // namespace actlib::Numbers::Steps
#endif /* GroupedRepetition */
