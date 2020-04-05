#ifndef Precision_hpp
#define Precision_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolSteps.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers { namespace Steps {
class Precision : public Protocol {
  public:
    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              std::unique_ptr<Range> range,
              std::vector<double> distribution);

    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              std::unique_ptr<Range> range,
              std::vector<double> distribution,
              int initialSelection);

    ~Precision();

    int getNumber() override;
    void reset() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    std::unique_ptr<Range> m_range;
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
}}} // namespace actlib::Numbers::Steps

#endif /* Precision_hpp */
