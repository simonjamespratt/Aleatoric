#ifndef Basic_hpp
#define Basic_hpp

#include "IUniformGenerator.hpp"
#include "ProtocolInterface.hpp"
#include "Range.hpp"

namespace actlib { namespace Numbers {
// TODO: document how Basic needs the generator to be instantiated (i.e.
// what distribution it needs). It needs range.start and range.end:
// generator(std::mt19937 &engine, int rangeStart, int rangeEnd)
class Basic : public Protocol {
  public:
    Basic(IUniformGenerator &generator, Range &range);
    ~Basic();
    int getNumber() override;
    void reset() override;

  private:
    actlib::Numbers::Range &_range;
    actlib::Numbers::IUniformGenerator &_generator;
};
}} // namespace actlib::Numbers

#endif /* Basic_hpp */
