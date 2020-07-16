#ifndef Precision_hpp
#define Precision_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"

namespace aleatoric {
class Precision : public NumberProtocol {
  public:
    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              Range range,
              std::vector<double> distribution);

    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              Range range,
              std::vector<double> distribution,
              int initialSelection);

    ~Precision();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

    void setRange(Range newRange) override;
    Range getRange() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    Range m_range;
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
} // namespace aleatoric

#endif /* Precision_hpp */
