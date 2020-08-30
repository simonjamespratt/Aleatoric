#ifndef DurationsProducer_hpp
#define DurationsProducer_hpp

#include "DurationProtocol.hpp"
#include "NumberProtocol.hpp"

namespace aleatoric {
class DurationsProducer {
  public:
    DurationsProducer(std::unique_ptr<DurationProtocol> durationProtocol,
                      std::unique_ptr<NumberProtocol> numberProtocol);
    ~DurationsProducer();

    int getDuration();

    std::vector<int> getCollection(int size);

    void reset();

    NumberProtocolParameters::Protocols getParams();

    void setParams(NumberProtocolParameters::Protocols newParams);

    void setNumberProtocol(std::unique_ptr<NumberProtocol> numberProtocol);

    void
    setDurationProtocol(std::unique_ptr<DurationProtocol> durationProtocol);

  private:
    std::unique_ptr<DurationProtocol> m_durationProtocol;
    std::unique_ptr<NumberProtocol> m_numberProtocol;
    int m_durationCollectionSize;
};

} // namespace aleatoric

#endif /* DurationsProducer_hpp */
