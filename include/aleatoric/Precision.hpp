#ifndef Precision_hpp
#define Precision_hpp

#include "IDiscreteGenerator.hpp"
#include "NumberProtocol.hpp"
#include "Range.hpp"

namespace aleatoric {
class Precision : public NumberProtocol {
  public:
    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              std::unique_ptr<Range> range,
              std::vector<double> distribution);

    Precision(std::unique_ptr<IDiscreteGenerator> generator,
              std::unique_ptr<Range> range,
              std::vector<double> distribution,
              int initialSelection);

    ~Precision();

    int getIntegerNumber() override;

    double getDecimalNumber() override;

    void reset() override;

  private:
    std::unique_ptr<IDiscreteGenerator> m_generator;
    std::unique_ptr<Range> m_range;
    int m_initialSelection;
    bool m_haveInitialSelection;
    bool m_haveRequestedFirstNumber;
};
} // namespace aleatoric

#endif /* Precision_hpp */
