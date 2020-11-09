#ifndef Utilities_hpp
#define Utilities_hpp

#include "IDiscreteGenerator.hpp"

#include <memory>
#include <tuple>

namespace aleatoric { namespace Utilities {

// Range is inclusive
bool numberIsOutOfRange(int number, int rangeStart, int rangeEnd);

class SeriesPrinciple {
  public:
    SeriesPrinciple();
    ~SeriesPrinciple();

    int getNumber(std::unique_ptr<IDiscreteGenerator> &generator);

    bool seriesIsComplete(std::unique_ptr<IDiscreteGenerator> &generator);

    void resetSeries(std::unique_ptr<IDiscreteGenerator> &generator);
};

}} // namespace aleatoric::Utilities

#endif /* Utilities_hpp */
