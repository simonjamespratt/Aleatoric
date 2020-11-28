#ifndef Subset_hpp
#define Subset_hpp

#include "IDiscreteGenerator.hpp"
#include "IUniformGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

#include <memory>

namespace aleatoric {
class SeriesPrinciple;

class Subset : public NumberProtocol {
  public:
    Subset(std::unique_ptr<IUniformGenerator> uniformGenerator,
           std::unique_ptr<IDiscreteGenerator> discreteGenerator);

    Subset(std::unique_ptr<IUniformGenerator> uniformGenerator,
           std::unique_ptr<IDiscreteGenerator> discreteGenerator,
           Range range,
           int subsetMin,
           int subsetMax);

    ~Subset();
    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void setParams(NumberProtocolParameters newParams) override;

    NumberProtocolParameters getParams() override;

  private:
    std::unique_ptr<IUniformGenerator> m_uniformGenerator;
    std::unique_ptr<IDiscreteGenerator> m_discreteGenerator;
    Range m_range;
    int m_subsetMin;
    int m_subsetMax;
    std::unique_ptr<SeriesPrinciple> m_seriesPrinciple;
    std::vector<int> m_subset;
    void setSubset();
    void checkSubsetValues(const int &subsetMin,
                           const int &subsetMax,
                           const Range &range);
    void initialise();
};
} // namespace aleatoric
#endif /* Subset_hpp */
