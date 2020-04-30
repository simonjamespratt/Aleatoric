#ifndef Multiples_hpp
#define Multiples_hpp

#include "DurationProtocol.hpp"
#include "IUniformGenerator.hpp"
#include "Range.hpp"

namespace actlib { namespace TimeDomain {
class Multiples : public DurationProtocol {
  public:
    Multiples(int baseIncrement, const actlib::Numbers::Range &range);

    Multiples(int baseIncrement,
              const actlib::Numbers::Range &range,
              double deviationFactor,
              std::unique_ptr<actlib::Numbers::IUniformGenerator> generator);

    Multiples(int baseIncrement, const std::vector<int> &multipliers);

    Multiples(int baseIncrement,
              const std::vector<int> &multipliers,
              double deviationFactor,
              std::unique_ptr<actlib::Numbers::IUniformGenerator> generator);

    ~Multiples();
    int getCollectionSize() override;
    int getDuration(int index) override;

  private:
    std::vector<int> m_durations;
    double m_deviationFactor;
    bool m_hasDeviationFactor;
    std::unique_ptr<actlib::Numbers::IUniformGenerator> m_generator = nullptr;
};
}}     // namespace actlib::TimeDomain
#endif /* Multiples_hpp */
