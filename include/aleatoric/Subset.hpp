#ifndef Subset_hpp
#define Subset_hpp

#include "IDiscreteGenerator.hpp"
#include "IUniformGenerator.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"
#include "Utilities.hpp"

#include <memory>

namespace aleatoric {
class Subset : public NumberProtocol {
  public:
    Subset(std::unique_ptr<IUniformGenerator> uniformGenerator,
           std::unique_ptr<IDiscreteGenerator> discreteGenerator,
           std::unique_ptr<Range> range,
           int subsetMin,
           int subsetMax);

    ~Subset();
    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

  private:
    std::unique_ptr<IUniformGenerator> m_uniformGenerator;
    std::unique_ptr<IDiscreteGenerator> m_discreteGenerator;
    std::unique_ptr<Range> m_range;
    Utilities::SeriesPrinciple m_seriesPrinciple;
    int m_subsetMin;
    int m_subsetMax;
    std::vector<int> m_subset;
    void setSubset();
};
} // namespace aleatoric
#endif /* Subset_hpp */
