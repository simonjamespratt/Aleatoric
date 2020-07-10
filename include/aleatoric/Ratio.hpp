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
          std::unique_ptr<Range> range,
          std::vector<int> ratios);

    ~Ratio();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    std::unique_ptr<Range> m_range;
    std::vector<int> m_selectables;
    Utilities::SeriesPrinciple m_seriesPrinciple;
};
} // namespace aleatoric

#endif /* Ratio_hpp */
