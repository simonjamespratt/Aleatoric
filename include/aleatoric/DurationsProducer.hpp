#ifndef DurationsProducer_hpp
#define DurationsProducer_hpp

#include "DurationProtocol.hpp"
#include "NumberProtocol.hpp"

namespace aleatoric {
class DurationsProducer {
  public:
    DurationsProducer(DurationProtocol &durationProtocol,
                      std::unique_ptr<NumberProtocol> numberProtocol);
    ~DurationsProducer();

    int getDuration();

    std::vector<int> getCollection(int size);

    void reset();

  private:
    DurationProtocol &m_durationProtocol;
    std::unique_ptr<NumberProtocol> m_numberProtocol;
};

} // namespace aleatoric

#endif /* DurationsProducer_hpp */
