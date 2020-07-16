#ifndef Ratio_hpp
#define Ratio_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"
#include "Utilities.hpp"

namespace aleatoric {
class Ratio : public NumberProtocol {
  public:
    Ratio(std::unique_ptr<IDiscreteGenerator> generator,
          Range range,
          std::vector<int> ratios);

    ~Ratio();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

    void setRange(Range newRange) override;
    Range getRange() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    Range m_range;
    std::vector<int> m_ratios;
    std::vector<int> m_selectables;
    Utilities::SeriesPrinciple m_seriesPrinciple;
    void setSelectables();
};
} // namespace aleatoric

#endif /* Ratio_hpp */
