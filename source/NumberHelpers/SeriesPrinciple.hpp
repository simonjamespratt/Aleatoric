#ifndef SeriesPrinciple_hpp
#define SeriesPrinciple_hpp

#include "IDiscreteGenerator.hpp"

#include <memory>

namespace aleatoric {
class SeriesPrinciple {
  public:
    SeriesPrinciple();
    ~SeriesPrinciple();

    int getNumber(std::unique_ptr<IDiscreteGenerator> &generator);

    bool seriesIsComplete(std::unique_ptr<IDiscreteGenerator> &generator);

    void resetSeries(std::unique_ptr<IDiscreteGenerator> &generator);
};
} // namespace aleatoric

#endif /* SeriesPrinciple_hpp */
