#ifndef Precision_hpp
#define Precision_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"
#include "Range.hpp"

namespace aleatoric {
class Precision : public NumberProtocol {
  public:
    Precision(std::unique_ptr<IDiscreteGenerator> generator);
    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              Range range,
              std::vector<double> distribution);

    ~Precision();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

    void setParams(NumberProtocolParameters newParams) override;

    NumberProtocolParameters getParams() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    Range m_range;
    void checkDistributionIsValid(const std::vector<double> &distribution);
    void checkDistributionMatchesRange(const std::vector<double> &distribution,
                                       const Range &range);
};
} // namespace aleatoric

#endif /* Precision_hpp */
