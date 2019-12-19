#ifndef Producer_hpp
#define Producer_hpp

#include "ProtocolInterface.hpp"

namespace actlib { namespace Numbers {
class Producer {
  public:
    Producer(Protocol &protocol);
    ~Producer();
    int getNumber();

  private:
    Protocol &_protocol;
};
}} // namespace actlib::Numbers

#endif /* Producer_hpp */
