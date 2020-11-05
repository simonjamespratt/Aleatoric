#ifndef GroupedRepetition_hpp
#define GroupedRepetition_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"
#include "Utilities.hpp"

namespace aleatoric {
class GroupedRepetition : public NumberProtocol {
  public:
    GroupedRepetition(std::unique_ptr<IDiscreteGenerator> numberGenerator,
                      std::unique_ptr<IDiscreteGenerator> groupingGenerator);

    GroupedRepetition(std::unique_ptr<IDiscreteGenerator> numberGenerator,
                      std::unique_ptr<IDiscreteGenerator> groupingGenerator,
                      Range range,
                      std::vector<int> groupings);

    ~GroupedRepetition();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

    void setParams(NumberProtocolParameters newParams) override;

    NumberProtocolParameters getParams() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_numberGenerator;
    std::unique_ptr<IDiscreteGenerator> m_groupingGenerator;
    Range m_range;
    std::vector<int> m_groupings;
    Utilities::SeriesPrinciple m_seriesPrinciple;
    int m_groupingCount;
    int m_currentReturnableNumber;
    void initialise();
};
} // namespace aleatoric
#endif /* GroupedRepetition */
