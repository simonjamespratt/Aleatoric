#ifndef ProducerTimeDomain_hpp
#define ProducerTimeDomain_hpp

#include "DurationProtocol.hpp"
#include "ProtocolSteps.hpp"

namespace actlib { namespace TimeDomain {
class Producer {
  public:
    Producer(actlib::TimeDomain::DurationProtocol &durationProtocol,
             std::unique_ptr<actlib::Numbers::Steps::Protocol> numberProtocol);
    ~Producer();

    int getDuration();

    std::vector<int> getCollection(int size);

    void reset();

  private:
    actlib::TimeDomain::DurationProtocol &m_durationProtocol;
    std::unique_ptr<actlib::Numbers::Steps::Protocol> m_numberProtocol;
};

}} // namespace actlib::TimeDomain

#endif /* ProducerTimeDomain_hpp */
