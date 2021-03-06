#ifndef DurationsProducer_hpp
#define DurationsProducer_hpp

#include "DurationProtocol.hpp"
#include "NumberProtocol.hpp"
#include "NumberProtocolParameters.hpp"

#include <functional>
#include <map>

namespace aleatoric {
class DurationsProducer {
  public:
    DurationsProducer(std::unique_ptr<DurationProtocol> durationProtocol,
                      std::unique_ptr<NumberProtocol> numberProtocol);
    ~DurationsProducer();

    int getDuration();

    std::vector<int> getCollection(int size);

    std::vector<int> getSelectableDurations();

    NumberProtocolParams getParams();

    void setParams(NumberProtocolParams newParams);

    int addListenerForParamsChange(std::function<void()> callback);

    void removeListenerForParamsChange(int callbackId);

    void setNumberProtocol(std::unique_ptr<NumberProtocol> numberProtocol);

    void
    setDurationProtocol(std::unique_ptr<DurationProtocol> durationProtocol);

  private:
    std::unique_ptr<DurationProtocol> m_durationProtocol;
    std::unique_ptr<NumberProtocol> m_numberProtocol;
    int m_durationCollectionSize;
    std::map<int, std::function<void()>> m_paramsChangeListeners;
    int m_listenersIdCounter {0};
    void notifyParamsChangeListeners();
    int getNewId();
};

} // namespace aleatoric

#endif /* DurationsProducer_hpp */
