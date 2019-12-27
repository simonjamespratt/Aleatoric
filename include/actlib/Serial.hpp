#ifndef Serial_hpp
#define Serial_hpp

#include "IDiscreteGenerator.hpp"
#include "ProtocolInterface.hpp"
#include "Range.hpp"
#include <random>

namespace actlib { namespace Numbers {
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
