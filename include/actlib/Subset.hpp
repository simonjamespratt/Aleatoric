#ifndef Subset_hpp
#define Subset_hpp

#include "IDiscreteGenerator.hpp"
#include "IUniformGenerator.hpp"
#include "ProtocolSteps.hpp"
#include "Range.hpp"
#include "Utilities.hpp"

#include <memory>

namespace actlib { namespace Numbers { namespace Steps {
class Subset : public Protocol {
  public:
    Subset(std::unique_ptr<IUniformGenerator> uniformGenerator,
           std::unique_ptr<IDiscreteGenerator> discreteGenerator,
           std::unique_ptr<actlib::Numbers::Range> range,
           int subsetMin,
           int subsetMax);

    ~Subset();
    int getNumber() override;
    void reset() override;

  private:
    std::unique_ptr<IUniformGenerator> m_uniformGenerator;
    std::unique_ptr<IDiscreteGenerator> m_discreteGenerator;
    std::unique_ptr<actlib::Numbers::Range> m_range;
    actlib::Utilities::SeriesPrinciple m_seriesPrinciple;
    int m_subsetMin;
    int m_subsetMax;
    std::vector<int> m_subset;
    void setSubset();
};
}}}    // namespace actlib::Numbers::Steps
#endif /* Subset_hpp */
