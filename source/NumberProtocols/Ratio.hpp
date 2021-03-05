#ifndef Ratio_hpp
#define Ratio_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
class SeriesPrinciple;

class Ratio : public NumberProtocol {
  public:
    Ratio(std::unique_ptr<IDiscreteGenerator> generator);

    Ratio(std::unique_ptr<IDiscreteGenerator> generator,
          Range range,
          std::vector<int> ratios);

    ~Ratio();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void setParams(NumberProtocolConfig newParams) override;

    NumberProtocolConfig getParams() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    Range m_range;
    std::vector<int> m_ratios;
    std::vector<int> m_selectables;
    std::unique_ptr<SeriesPrinciple> m_seriesPrinciple;
    void setSelectables();
    void checkRangeAndRatiosMatch(const Range &range,
                                  const std::vector<int> &ratios);
    void initialise();
};
} // namespace aleatoric

#endif /* Ratio_hpp */
