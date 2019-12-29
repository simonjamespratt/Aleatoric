#ifndef Serial_hpp
#define Serial_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolInterface.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers {
// TODO: document how Serial needs the generator to be instantiated (i.e.
// what distribution it needs). It needs a uniform distribution of the size of
// the range.size:
// generator(std::mt19937 &engine, int vectorSize, double uniformValue)
class Serial : public Protocol {
  public:
    Serial(IDiscreteGenerator &generator, Range &range);
    ~Serial();
    int getNumber() override;
    void reset() override;

  private:
    actlib::Numbers::Range &_range;
    actlib::Numbers::IDiscreteGenerator &_generator;
    bool seriesIsComplete();
};
}} // namespace actlib::Numbers

#endif /* Serial_hpp */
