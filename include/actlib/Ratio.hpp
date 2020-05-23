#ifndef Ratio_hpp
#define Ratio_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolSteps.hpp"
#include "Range.hpp"
#include "Utilities.hpp"

namespace actlib { namespace Numbers { namespace Steps {
class Ratio : public Protocol {
  public:
    Ratio(std::unique_ptr<IDiscreteGenerator> generator,
          std::unique_ptr<Range> range,
          std::vector<int> ratios);

    ~Ratio();

    int getNumber() override;
    void reset() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    std::unique_ptr<Range> m_range;
    std::vector<int> m_selectables;
    actlib::Utilities::SeriesPrinciple m_seriesPrinciple;
};
}}} // namespace actlib::Numbers::Steps

#endif /* Ratio_hpp */
