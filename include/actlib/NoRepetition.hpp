#ifndef NoRepetition_hpp
#define NoRepetition_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolInterface.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers {
// TODO: document how NoRepetition needs the generator to be instantiated (i.e.
// what distribution it needs). It needs a uniform distribution of the size of
// the range.size:
// generator(std::mt19937 &engine, int vectorSize, double uniformValue)
class NoRepetition : public Protocol {
  public:
    NoRepetition(IDiscreteGenerator &generator, Range &range);
    ~NoRepetition();
    int getNumber() override;
    void reset() override;

  private:
    actlib::Numbers::Range &_range;
    actlib::Numbers::IDiscreteGenerator &_generator;
};
}} // namespace actlib::Numbers

#endif /* NoRepetition_hpp */
