// TODO: rename this to just "Protocol"

#ifndef ProtocolInterface_hpp
#define ProtocolInterface_hpp

namespace actlib { namespace Numbers {
class Protocol {
  public:
    virtual int getNumber() = 0;
    virtual void reset() = 0;
    virtual ~Protocol() = default;
};
}} // namespace actlib::Numbers

#endif /* ProtocolInterface_hpp */
